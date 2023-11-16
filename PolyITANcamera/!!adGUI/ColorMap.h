#include <assert.h>
#include "../!!adGlobals/vector_math.h"
#include <string>

class ColorMap
{
public:
	ColorMap(std::string str, int x, int y, float _v_min, float _v_max);
	void Draw();
	bool Clicked(int button, int state, int x, int y);
	void Hover(int x, int y);
	void SetBoxWidth(int _b_w);
	void SetBoxHeight(int _b_s);
protected:
	int posx;
	int posy;
	std::string _text;
	int _box_width;
	int _box_height;
	int _text_sep;
	float _text_height;
	float _text_width;
	float _text_scale;

	float val_min;
	float val_max;

	Vec4 active_col;
	Vec4 inactive_col;
	Vec3 start_col;
	Vec3 end_col;
	bool active;
};
