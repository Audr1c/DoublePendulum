typedef struct {
    int r, g, b, a, n;
    int padding[3]; 
} Pixel;

// Kernel OpenCL : Trace un segment avec atomic_add
__kernel void draw_segment(
    __global Pixel *img, int img_w, int img_h,
    int x1, int y1, int x2, int y2, // Coordonnées des deux extrémités du segment
    int radius,
    int r, int g, int b, int a)     // Couleur appliquée
{
    // 1. Définition de la Bounding Box du segment entier
    int min_x = min(x1, x2) - radius;
    int max_x = max(x1, x2) + radius;
    int min_y = min(y1, y2) - radius;
    int max_y = max(y1, y2) + radius;

    // 2. Assignation : 1 thread = 1 pixel de cette zone
    int px = min_x + get_global_id(0);
    int py = min_y + get_global_id(1);

    if (px < 0 || px >= img_w || py < 0 || py >= img_h) return;
    if (px > max_x || py > max_y) return;

    // 3. Calcul de la distance du pixel (px, py) au segment (x1, y1) -> (x2, y2)
    float l2 = (float)((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
    float t = 0.0f;
    if (l2 > 0.0f) {
        t = max(0.0f, min(1.0f, (float)((px - x1)*(x2 - x1) + (py - y1)*(y2 - y1)) / l2));
    }
    
    float proj_x = x1 + t * (x2 - x1);
    float proj_y = y1 + t * (y2 - y1);
    
    float dx = px - proj_x;
    float dy = py - proj_y;

    // 4. Application si le pixel est dans l'épaisseur du trait
    if (dx*dx + dy*dy <= radius*radius) {
        int idx = py * img_w + px;
        
        // Exclusivié sur un meme segment, mais si on fait plusieurs segment en meme temps alors on a besoin d'atomic.
        // ca reste rapide car c'est uniquement si conflit et le thread (1pix) est mis en pause. 
        atomic_add(&img[idx].r, r);
        atomic_add(&img[idx].g, g);
        atomic_add(&img[idx].b, b);
        atomic_add(&img[idx].a, a);
        atomic_add(&img[idx].n, 1);
    }
}