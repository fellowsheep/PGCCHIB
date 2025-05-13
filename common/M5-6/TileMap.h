class TileMap {
    float z;               // caso de eventual de vários tilemaps sobrepostos
    unsigned int tid;      // indicação do tileset utilizado
    int width, height;     // dimensões da matriz
    unsigned char *map; // mapa com ids dos tiles que formam o cenário

    
public:
    TileMap(int w, int h, unsigned char initWith) {
        this->map = new unsigned char [w*h];
        this->width = w;
        this->height = h;
        this->z = 0.0f;
        this->tid = 0;
    }
    
//    TileMap(const TileMap &tm) {
//        map = new unsigned char[tm.width * tm.height];
//        this->width = tm.width;
//        this->height = tm.height;
//        this->z = tm.z;
//        this->tid = tm.tid;
////        for (int i=0; this->width * this->height; i++) {
////            this->map[i] = tm.map[i];
////        }
//        memcpy(this->map, tm.map, tm.width*tm.height);
//    }
    
    unsigned char* getMap() {
        return this->map;
    }
    
    int getWidth() {
        return this->width;
    }
    
    int getHeight() {
        return this->height;
    }
    
    int getTile(int col, int row) {
        return this->map[col + row * this->width];
    }
    
    void setTile(int col, int row, unsigned char tile) {
        this->map[col + row * this->width] = tile;
    }
    
    int getTileSet() {
        return this->tid;
    }
    
    float getZ() {
        return this->z;
    }
    
    void setZ(float z){
        this->z = z;
    }
    
    void setTid(int tid) {
        this->tid = tid;
    }
    
};

