#pragma once


#define BUFFER_SIZE 5

class RingBufferWithSum
{
public:
  uint32_t currentSum = 0; /**< Sum of current round since index 0. */
  uint16_t values[BUFFER_SIZE] = {0};
  uint16_t head = 0; /**< The next entry that will be used for push_front(). */

public:

  void add(uint16_t value)
  {
    //only works because the buffer is 0-initialized
    currentSum -= values[head];
    currentSum += value;
    values[head] = value;
    head = (head + 1) % BUFFER_SIZE;
  }

  /**
   * Returns the average of all entries in O(1). If the buffer is empty, the
   * zero element is returned.
   */
  float average() const
  {
    return currentSum / (float)BUFFER_SIZE;
  }

};

