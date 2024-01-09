//this code works with all microcontrollers to read rotary encoder's rotation (clicks) in both directions

#define DT_PIN        7
#define CL_PIN        8

bool high_detected = false;
bool low_detected = false;
bool mid_detected = false;
uint8_t rotary_status = 1;
uint8_t pre_rotary_status = 1;

uint8_t read_rotary_dt(void)
{
  if(digitalRead(DT_PIN) == HIGH)
  {
    return 1;
  }
  return 0;
}

uint8_t read_rotary_clk(void)
{
  if(digitalRead(CL_PIN) == HIGH)
  {
    return 1;
  }
  return 0;
}

void read_encoder(void)
{
  uint8_t rclk = read_rotary_clk();
  uint8_t rdt = read_rotary_dt();
  //check if initial state is detected
  if((rclk != 1) || (rdt != 1))
  {
    //check if high or low is already detected
    if((!high_detected) && (!low_detected))
    {
      if((rclk == 1) && (rdt == 0))
      {
        high_detected = true;
        low_detected = false;
        mid_detected = false;
      }
      else if((rclk == 0) && (rdt == 1))
      {
        low_detected = true;
        high_detected = false;
        mid_detected = false;
      }
    }
    else
    {
      if(!mid_detected)
      {
        if((rclk == 0) && (rdt == 0))
        {
          if(low_detected)
          {
            //check the previous detection
            mid_detected = true;
          }
          else if(high_detected)
          {
            //check the previous detection
            mid_detected = true;
          }
        }
      }
      else
      {
        if((rclk == 1) && (rdt == 0))
        {
          if(low_detected)
          {
            //do the rotary low operation (counter clockwise)
            bk8000l_vol_down();
            high_detected = false;
            low_detected = false;
            mid_detected = false;
          }
        }
        else if((rclk == 0) && (rdt == 1))
        {
          if(high_detected)
          {
            //do the rotary high operation (clockwise)
            bk8000l_vol_down();
            high_detected = false;
            low_detected = false;
            mid_detected = false;
          }
        }
      }
    }
  }
  else
  {
    //check if its high or low or reset
    if((rclk == 1) && (rdt == 1))
    {
      high_detected = false;
      low_detected = false;
      mid_detected = false;
    }
  }
  pre_rotary_status = rotary_status;
}
