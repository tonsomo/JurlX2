class sprite
{
	private:
	int	x,y;
	int	xdir,ydir;
	int object;
	int alive;
	int owner;
	int nexttime;
	int lasttime;

	public:
	int x() { return x };
	void x(int i) { x=i };
}