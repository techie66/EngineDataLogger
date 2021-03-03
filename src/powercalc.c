float trailing_average_power(const double speed, const struct timeval currtime, float weight_kg=300, int periods=20) {
	// TODO Calculate power by comparing deltas
	static float last_speed = 0;
	static struct timeval last_time = currtime;
	float speed_delta = last_speed - speed;
	struct timeval time_delta;


	return 50.0
}

/** @Alexey Frunze
	https://stackoverflow.com/questions/15846762/timeval-subtract-explanation
	Shamelessly copied
**/
int timeval_subtract(struct timeval *result, struct timeval *x, struct timeval *y)
{
  struct timeval xx = *x;
  struct timeval yy = *y;
  x = &xx; y = &yy;

  if (x->tv_usec > 999999)
  {
    x->tv_sec += x->tv_usec / 1000000;
    x->tv_usec %= 1000000;
  }

  if (y->tv_usec > 999999)
  {
    y->tv_sec += y->tv_usec / 1000000;
    y->tv_usec %= 1000000;
  }

  result->tv_sec = x->tv_sec - y->tv_sec;

  if ((result->tv_usec = x->tv_usec - y->tv_usec) < 0)
  {
    result->tv_usec += 1000000;
    result->tv_sec--; // borrow
  }

  return result->tv_sec < 0;
}
