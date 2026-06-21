#ifndef WINDOW_HH
#define WINDOW_HH

#include <string>
#include "assert.hh"

#define FENSTER_HEADER
#include "fenster.h"

#include "geometry.hh"

namespace pro2 {

/**
 * @enum ModKey
 *
 * Enumeració per a les 4 tecles de control: Ctrl, Shift, Alt, i Meta.
 */
enum ModKey { Ctrl = 1, Shift = 2, Alt = 4, Meta = 8 };

/**
 * @enum Colors
 *
 * Enumeració de colors amb els colors més simples (els 8 colors de la paleta
 * EGA).
 */

typedef uint32_t Color;

const Color black = 0x00000000;
const Color red = 0x00ff0000;
const Color green = 0x0000ff00;
const Color blue = 0x000000ff;
const Color yellow = 0x00ffff00;
const Color magenta = 0x00ff00ff;
const Color cyan = 0x0000ffff;
const Color white = 0x00ffffff;

/**
 * @enum Keys
 *
 * Enumeració amb els codis de les tecles que es poden passar al mètode
 * `Window::is_key_down` per consultar l'estat d'una tecla.
 */
enum Keys {
    Space = 32,
    Backspace = 8,
    Delete = 127,
    End = 5,
    Escape = 27,
    Home = 2,
    Insert = 26,
    PageDown = 4,
    PageUp = 3,
    Return = 10,
    Tab = 9,
    // Arrows,
    Up = 17,
    Down = 18,
    Right = 19,
    Left = 20,
};

/**
 * @class Window
 *
 * La classe `Window` permet obrir finestres en mode gràfic a Linux, MacOS i
 * Windows. Té uns pocs mètodes que permeten fer programes simples que mostren
 * gràfics, com petits jocs o editors.
 */
class Window {
 private:
    /**
     * @brief L'estructura de dades que Fenster necessita per treballar
     */
    int     last_keys_[256];
    int     last_mouse_;
    fenster fenster_;

    /**
     * @brief El buffer de píxels que es reserva com a zona de pintat
     *
     * Cada píxel té 32 bits, o 4 bytes, i els 3 bytes de menys pes són els
     * valors (entre 0 i 255) dels canals R, G i B (red, green i blue).
     */
    uint32_t *pixels_;

    /**
     * @brief Mida del buffer en bytes
     */
    size_t pixels_size_;

    /**
     * @brief Paràmetre de `zoom` per a aquesta finestra
     */
    int zoom_ = 1;

    /**
     * @brief Instant de l'últim fotograma (epoch)
     */
    int64_t last_time_;

    /**
     * @brief Comptador de frames (o fotogrames)
     */
    int frame_count_ = 0;

    /**
     * @brief Fotogrames per segon (FPS)
     */
    uint8_t fps_ = 60;

    // Càmera

    /**
     * @brief Camera topleft
     */
    Pt topleft_ = {0, 0};
    Pt topleft_target_ = {0, 0};

    /**
     * @brief Aquest mètode actualitza la càmera en funció de la velocitat.
     */
    void update_camera_();

    /**
     * @brief Determina si la càmera està en moviment (la seva posició final és
     * diferent de l'actual).
     */
    bool camera_moving_() const {
        return topleft_.x != topleft_target_.x || topleft_.y != topleft_target_.y;
    }

    /**
     * @brief Constant de la velocitat a la qual la càmera es desplaça a la nova
     * posició.
     */
    static constexpr int camera_speed_ = 8;

 public:
    /**
     * @brief Construeix una finestra amb títol, amplada i alçada.
     *
     * El constructor obre una finestra, i el destructor la tanca.
     *
     * El paràmetre `zoom` permet visualitzar amb més comoditat contingut
     * pixelat. Amb `zoom = 1` cada píxel de la finestra es correspon amb un
     * píxel de la pantalla. Amb `zoom = 3`, cada píxel de la finestra es
     * converteix en un quadrat de 3x3 píxels a la finestra.
     *
     * @param title El títol de la finestra (un literal de cadena de caràcters)
     * @param width L'amplada de la finestra en píxels.
     * @param height L'alçada de la finestra en píxels.
     * @param zoom El factor d'augment de cada píxel. (És opcional, si no hi ha
     * 4t paràmetre pren valor 1)
     */
    Window(std::string title, int width, int height, int zoom = 1);

    /**
     * @brief Destrueix una finestra, és a dir, tanca la finestra oberta al
     * constructor.
     *
     */
    ~Window() {
        fenster_close(&fenster_);
        delete[] pixels_;
    }

    /**
     * @brief Retorna l'amplada de la finestra.
     *
     */
    int width() const {
        return fenster_.width / zoom_;
    }

    /**
     * @brief Retorna l'alçada de la finestra.
     *
     */
    int height() const {
        return fenster_.height / zoom_;
    }

    /**
     * @brief Gestiona les tasques necessàries per passar al següent fotograma.
     *
     * En tot programa gràfic és necessari: 1) pintar en una superfície,
     * típicament en memòria, 2) transferir el que s'ha pintat a la pantalla, 3)
     * processar esdeveniments ocorreguts com pressió de tecles o moviment del
     * ratolí i actualitzar-ne l'estat, i 4) esperar el temps que quedi fins al
     * següent fotograma (en funció de la velocitat de refresc, que sol ser de
     * 60Hz, cosa que equival a 16ms per fotograma).
     *
     * `next_frame` fa totes aquestes coses en una sola crida. A més retorna
     * `false` quan s'ha clicat el botó de tancar la finestra (típicament a dalt
     * a la dreta, i amb una "x"), de manera que es pugui saber si cal continuar
     * en un bucle de pintat de fotogrames.
     *
     * L'ús típic és el següent:
     * ```c++
     * while (window.next_frame()) {
     *     // usar els mètodes de detecció de tecles o ratolí, i set_pixel per
     * pintar...
     * }
     * ```
     * És a dir, fins que no es tanqui la finestra cridem mètodes de la finestra
     * per fer operacions que resultin en el pintat de la finestra d'una certa
     * manera i `next_frame` se'n fa càrrec de la resta.
     *
     * Pel que fa al teclat i ratolí, `next_frame` recull tots els esdeveniments
     * (pressió i alliberament de tecles, clics i moviment del ratolí) que han
     * ocorregut entre el fotograma anterior i l'actual, i amb tots ells
     * actualitza l'estat final de cada tecla, botó del ratolí i posició. Així
     * doncs, l'usuari de la classe `Window` té accés a l'estat exacte de les
     * tecles i el ratolí en l'instant en què es passa al fotograma actual, i
     * aquest estat es conserva fix mentre transcorre el temps entre el
     * fotograma actual i el següent, en el qual `next_frame` torna a revisar
     * els esdeveniments ocorreguts en aquest interval de temps.
     *
     * @returns `true` si el programa ha de seguir (NO s'ha clicat el botó de
     * tancar la finestra), `false` en cas contrari.
     *
     */
    bool next_frame();

    /**
     * @brief Omple la finestra amb un color.
     *
     * Aquest mètode es pot cridar amb un color o bé sense paràmetres. Si es
     * crida sense paràmetres es pren el `color` per defecte, que és el negre
     * (`black`). En cas contrari s'usa el color indicat.
     *
     * @param color El color a utilitzar per pintar. Es pot usar un dels valors
     * de l'enumeració `Colors`, com `red`, o bé posar un enter en hexadecimal,
     * com 0x0084fb, que equival als 3 valors RGB (o Red-Green-Blue) que
     * conformen el color. Qualsevol "color picker" de la web sol mostrar el
     * color hexadecimal en la notació `#0084fb` (de CSS).
     */
    void clear(Color color = black);

    /**
     * @brief Retorna el comptador de fotogrames pintats fins al moment.
     *
     * Equival a la quantitat de vegades que s'ha cridat `next_frame`.
     * S'incrementa en 1 unitat a cada fotograma.
     *
     * Aquest valor és útil en fer animacions, ja que permet saber, d'una
     * seqüència d'imatges, quina caldria usar a cada moment.
     *
     * @returns Un enter que correspon al comptador de fotogrames mostrats des
     * que la finestra es va crear.
     *
     */
    int frame_count() const {
        return frame_count_;
    }

    /**
     * @brief Determina si una certa tecla estava premuda al fotograma anterior
     *
     * El mètode `next_frame` recull tots els esdeveniments de teclat i ratolí
     * que han ocorregut des de la crida anterior a `next_frame` (o des de la
     * creació de la finestra) i manté l'estat de totes les tecles i botons del
     * ratolí fix durant el fotograma actual. Així doncs, el mètode
     * `is_key_down` simplement consulta aquest estat, que es manté fix fins a
     * la següent crida a `next_frame`.
     *
     * Exemple:
     * ```c++
     * if (window.is_key_down('S')) { ... }
     * if (window.is_key_down('1')) { ... }
     * if (window.is_key_down(Key::Escape)) { ... }
     * ```
     *
     * @param code El codi de la tecla de la qual es vol saber si estava
     * premuda. El codi de la lletra és, o bé el codi ASCII de la lletra
     * majúscula corresponent, el codi ASCII del dígit corresponent, o bé un
     * dels valors de l'`enum` `Key`, que recull les tecles més típiques,
     * incloent-hi fletxes, return, esc, tab, etc.
     *
     * @returns `true` quan la tecla `code` estava premuda en començar el
     * fotograma actual.
     *
     */
    bool is_key_down(int code) const {
        return code >= 0 && code < 128 && fenster_.keys[code];
    }

    /**
     * @brief Determina si una certa tecla es va prémer entre el fotograma
     * anterior i l'actual
     *
     * (Al mètode `is_key_down` s'explica millor el funcionament dels
     * esdeveniments.)
     *
     * Exemple:
     * ```c++
     * if (window.was_key_pressed('S')) { ... }
     * if (window.was_key_pressed('1')) { ... }
     * if (window.was_key_pressed(Key::Escape)) { ... }
     * ```
     *
     * @param code El codi de la tecla de la qual es vol saber si estava
     * premuda. El codi de la lletra és, o bé el codi ASCII de la lletra
     * majúscula corresponent, el codi ASCII del dígit corresponent, o bé un
     * dels valors de l'`enum` `Key`, que recull les tecles més típiques,
     * incloent-hi fletxes, return, esc, tab, etc.
     *
     * @returns `true` quan la tecla `code` es va prémer entre el fotograma
     * anterior i l'actual.
     *
     */
    bool was_key_pressed(int code) const {
        return code >= 0 && code < 128 && !last_keys_[code] && fenster_.keys[code];
    }

    /**
     * @brief Determina si una certa tecla de control estava premuda al
     * fotograma anterior
     *
     * Mètode anàleg a `is_key_down` però per a les tecles de control següents:
     * Ctrl, Alt, Shift i Meta. Hi ha una enumeració de nom `ModKey` amb les 4
     * tecles: `ModKey::Ctrl`, `ModKey::Alt`, `ModKey::Shift`, i `ModKey::Meta`.
     *
     * @param key La tecla de la qual es vol consultar l'estat.
     *
     * @returns `true` si l'estat de la tecla era "premuda" en entrar al
     * fotograma actual.
     *
     */
    bool is_modkey_down(ModKey key) const {
        return fenster_.mod & uint8_t(key);
    }

    /**
     * @brief Determina si el botó esquerre va quedar en estat clicat al
     * fotograma anterior.
     *
     * Aquest mètode es comporta com `is_key_down`, consulta la documentació
     * d'`is_key_down` per saber com opera.
     *
     * @returns `true` si el botó del ratolí va quedar clicat al final del
     * fotograma actual.
     *
     */
    bool is_mouse_down() const {
        return bool(fenster_.mouse);
    }

    /**
     * @brief Determina si el botó esquerre del ratolí es va clicar entre el
     * fotograma anterior i l'actual.
     *
     * Aquest mètode es comporta com `was_key_pressed`, consulta la documentació
     * de `was_key_pressed` per saber com opera.
     *
     * @returns `true` si el botó del ratolí es va clicar entre el fotograma
     * anterior i l'actual.
     */
    bool was_mouse_pressed() const {
        return !last_mouse_ && bool(fenster_.mouse);
    }

    /**
     * @brief Retorna la posició del cursor del ratolí.
     *
     * @returns Una tupla de tipus `Pt`, amb camps `x` i `y`, que es corresponen
     * amb les coordenades de la posició del ratolí.
     */
    Pt mouse_pos() const;

    /**
     * @brief Espera que passi un nombre `ms` de mil·lisegons sense fer res.
     *
     * En aquest interval de temps el programa estarà esperant que el mètode
     * torni de la crida, i per tant no s'executarà cap instrucció.
     *
     * @param ms Nombre de mil·lisegons a esperar.
     */
    void sleep(int ms) const {
        fenster_sleep(ms);
    }

    /**
     * @brief Obté el color d'un píxel de la finestra.
     *
     * @param xy Coordenades del píxel de la pantalla del qual es vol saber el
     * color.
     * @returns El color del píxel a les coordenades indicades.
     */
    Color get_pixel(Pt xy) const {
        return fenster_pixel(&fenster_, xy.x * zoom_, xy.y * zoom_);
    }

    /**
     * @brief Canvia un píxel de la finestra.
     *
     * En realitat, `set_pixel` no canvia la finestra directament, sinó un
     * "buffer" intern que es bolca a la pantalla de cop en el moment de cridar
     * `next_frame`. Això és més eficient i maximitza el temps en què el
     * fotograma està immòbil a la pantalla mostrant una imatge fixa, ja que el
     * pintat podria trigar tant de temps que els fotogrames no es veurien
     * complets a la pantalla durant els 16ms (a 60Hz) en què han d'estar
     * visibles.
     *
     * @param xy Coordenades del píxel que es vol canviar
     * @param color Color que es vol posar al píxel indicat
     */
    void set_pixel(Pt xy, Color color);

    /**
     * @brief Canvia els FPS de refresc de la finestra.
     *
     * En funció de la velocitat de refresc de la pantalla que vulguem, el temps
     * a esperar entre que pintem un fotograma i el següent pot variar. Aquest
     * mètode calcula un temps d'espera entre una crida a `next_frame` i la
     * següent, perquè es produeixi exactament un nombre de fotogrames per
     * segon.
     *
     * @param fps Nombre de fotogrames per segon que es volen mostrar.
     *
     * @pre `fps` > 0 && `fps` < 240.
     */
    void set_fps(int fps) {
        assert(fps > 0 && fps < 240, "Els FPS han d'estar entre 1 i 239.");
        fps_ = fps;
    }

    /**
     * @brief Indica que la posició de la cantonada superior esquerra de la
     * finestra hauria de moure's segons el vector `desplazamiento`.
     *
     * La càmera no es mou instantàniament, sinó que es desplaça a la nova
     * posició a una velocitat constant.
     *
     * @param desplazamiento Vector de desplaçament
     */
    void move_camera(Pt desplazamiento) {
        if (!camera_moving_()) {
            topleft_target_.x = topleft_.x + desplazamiento.x;
            topleft_target_.y = topleft_.y + desplazamiento.y;
        }
    }

    /**
     * @brief Retorna la posició del centre de la càmera.
     *
     * @returns Un `Pt` amb les coordenades del centre de la càmera.
     */
    Pt camera_center() const {
        const int width = fenster_.width / zoom_;
        const int height = fenster_.height / zoom_;
        return {topleft_.x + width / 2, topleft_.y + height / 2};
    }

    Rect camera_rect() const {
        const int width = fenster_.width / zoom_;
        const int height = fenster_.height / zoom_;
        const int left = topleft_.x;
        const int top = topleft_.y;
        const int right = topleft_.x + width;
        const int bottom = topleft_.y + height;
        return {left, top, right, bottom};
    }

    /**
     * @brief Estableix la posició de la cantonada superior esquerra de la
     * càmera.
     *
     * Aquest mètode mou la càmera instantàniament a la nova posició.
     *
     * @param topleft La nova posició absoluta de la càmera, que s'aplica
     * instantàniament.
     */
    void set_camera_topleft(Pt topleft) {
        topleft_ = topleft;
        topleft_target_ = topleft;
    }

    /**
     * @brief Retorna la posició de la cantonada superior esquerra de la càmera.
     *
     * @returns Un `Pt` amb les coordenades de la cantonada superior esquerra de
     * la càmera.
     */
    Pt topleft() const {
        return topleft_;
    }
};

}  // namespace pro2

#endif
