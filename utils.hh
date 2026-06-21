#ifndef UTILS_HH
#define UTILS_HH

#include <vector>
#include "geometry.hh"
#include "window.hh"

namespace pro2 {

/**
 * @brief Dibuixa una línia horitzontal a la finestra.
 *
 * @param window Finestra a la qual es dibuixa la línia.
 * @param xini Coordenada x inicial.
 * @param xfin Coordenada x final.
 * @param y Coordenada y.
 * @param color Color de la línia (opcional, si no es posa s'assumeix `white`).
 */
void paint_hline(pro2::Window& window, int xini, int xfin, int y, pro2::Color color = pro2::white);

/**
 * @brief Dibuixa una línia vertical a la finestra.
 *
 * @param window Finestra a la qual es dibuixa la línia.
 * @param x Coordenada x.
 * @param yini Coordenada y inicial.
 * @param yfin Coordenada y final.
 * @param color Color de la línia (opcional, si no es posa s'assumeix `white`).
 */
void paint_vline(pro2::Window& window, int x, int yini, int yfin, pro2::Color color = pro2::white);

/**
 * @brief Dibuixa una imatge/textura a la finestra a partir d'una posició
 *
 * @param window Finestra a la que pintar
 * @param orig   Origen (cantonada de dalt a l'esquerra) del rectangle que forma el _sprite_
 * @param sprite Matriu de colors que representa la imatge (_sprite_).
 * @param mirror Si cal pintar girar la textura horitzontalment
 */
void paint_sprite(pro2::Window&                        window,
                  pro2::Pt                             orig,
                  const std::vector<std::vector<int>>& sprite,
                  bool                                 mirror);

}  // namespace pro2

#endif