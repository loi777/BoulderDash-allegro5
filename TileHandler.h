int ChecaTilePresso(mapa* jogo_info, int x, int y);
int ChecaTileVazio(mapa* jogo_info, int x, int y);

void TransformaTile(mapa* jogo_info, int x, int y, int boulder_type, int estado);
void MoverPara(mapa* jogo_info, int x1, int y1, int x2, int y2);

int TijoloEncantadoAtivavel(mapa* jogo_info);

void DeslizaTile(mapa* jogo_info, int x, int y);
void GravidadeHandler(mapa* jogo_info, strutc_sound* boulder_sound, int x, int y);