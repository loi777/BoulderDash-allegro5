struct_font* CarregarFonte();
void DestroiFonte(struct_font* boulder_font);

void AtualizarTexto(mapa* jogo_info, ALLEGRO_BITMAP* pixel_sheet, struct_font* boulder_font);
void DesenhaInfoScreen(struct_font* boulder_font);
void DesenhaTelaFinal(mapa* jogo_info, struct_font* boulder_font);