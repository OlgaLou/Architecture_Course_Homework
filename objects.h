#pragma once

struct point
{
  void set (int x, int y)
  {
    this->x = x;
    this->y = y;
  }

  int x = 0;
  int y = 0;
};

struct rectangle
{
  point left_bottom_corner;
  point right_top_corner;

  bool is_point_inside (point p)
  {
    return (  p.x >= left_bottom_corner.x && p.x <= right_top_corner.x
           && p.y >= left_bottom_corner.y && p.y <= right_top_corner.y);
  }
};

class movable_iface
{
public:
  movable_iface () {}
  virtual ~movable_iface () = default;

  virtual point get_position () const = 0;
  virtual point get_velocity () const = 0;
  virtual void set_position (const point &position) = 0;
};
