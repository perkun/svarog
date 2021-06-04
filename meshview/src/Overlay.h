#ifndef OVERLAY_H_
#define OVERLAY_H_


class Overlay
{
public:
	Overlay() = default;
	Overlay(int corner, float alpha, const char *imgui_name);
	~Overlay();

	int corner;
	float alpha;
	string text;

	char imgui_name[100];

	double default_timeout = 5.;
	double timeout;

	void display();
	void display(string new_text);
	void display_timeout();
	void display_timeout(string new_text);

	void reset(string new_text);
	void reset_timeout();
	void reset_timeout(string new_text);

private:

};

#endif /* OVERLAY_H_ */
