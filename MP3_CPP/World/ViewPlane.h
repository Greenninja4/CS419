#ifndef __VIEW_PLANE__
#define __VIEW_PLANE__

class ViewPlane{
    public:
        int hres;
        int vres;
        float pixelSize;
        float gamma;
        float inv_gamma;
        int max_depth;

        ViewPlane(void);
        ViewPlane(int hres, int vres, float pixelSize, float gamma);
        ViewPlane(int hres, int vres, float pixelSize, float gamma, int max_depth);
        ViewPlane(const ViewPlane& vp);
        ViewPlane& operator= (const ViewPlane& rhs);
        ~ViewPlane(void);

        void set_hres(const int h_res);
        void set_vres(const int v_res);
        void set_pixel_size(const float size);
        void set_gamma(const float g);
        void set_max_depth(const int max_depth);
};

#endif