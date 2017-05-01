#pragma once
#include <opencv2\core.hpp>

template < class C, class Tr, class... Args >
class Button
{
	typedef Tr(C::*eventMethod)(Args...);
	~Button() {}
	
	Button(C& target, eventMethod method) : m_target(target), m_method(method) {}
	///< A constructor.
	/// @param target The class member instance used to call the method.
	/// @param method The class member method pointer to register.

	bool operator () (Args&... args) const
	{
		(m_target.*(m_method))(args...);
	}
	///< Invokes the registered method with the provided arguments.
	/// @param args The arguments to transmit to the invoked method.

private:
	eventMethod m_method;
	C& m_target;
};


class OpencvButton
{
protected:
    bool selected{ false };
	Scalar colorUnselected{ 0, 255, 0 };
	Scalar colorSelected{ 0,255,255 };
	Point position;
	
	Rect mButtonRect;
	Size size{ 10,10 };
public:
	std::function<bool(void*, Point)> fun;
	OpencvButton(
		Point refPos,
		std::function<bool(void*, Point)> fn
	) :position(refPos), fun(fn)
	{
		mButtonRect = Rect(position, size);
	}
	void setSelected(bool pselected) { selected = pselected; };
	bool isSelected() { return selected; };
	virtual void callBack(Point) = 0;
	virtual void draw(Mat&) = 0;
	Rect& getRect() { return mButtonRect; };
	~OpencvButton() {};
};