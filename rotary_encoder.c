/***** this code works with all microcontrollers to read rotary encoder's rotation (clicks) in both directions
this code is written for arduino IDE *****/

#define DT_PIN        7      //pin number for encoder data pin
#define CL_PIN        8      //pin number for encoder clock pin

//these variables must be declared global
bool high_detected = false;
bool low_detected = false;
bool mid_detected = false;
uint8_t rotary_status = 1;
uint8_t pre_rotary_status = 1;

////// this function should be called initially once to make the encoder data and clock pin as input open drain (with pullup) //////
void init_encoder(void)
{
  pinMode(DT_PIN, INPUT_PULLUP);      //you can write hardware specific statement to make the IO input/open drain (with pullup)
  pinMode(CL_PIN, INPUT_PULLUP);      //you can write hardware specific statement to make the IO input/open drain (with pullup)
}

////// this function reads the encoder data input and returns the 0/1 integer value //////
uint8_t read_rotary_dt(void)
{
  if(digitalRead(DT_PIN) == HIGH)    //you can write hardware specific statement here to read the IO pin
  {
    return 1;
  }
  return 0;
}

////// this function reads the encoder clock input and returns the 0/1 integer value //////
uint8_t read_rotary_clk(void)
{
  if(digitalRead(CL_PIN) == HIGH)    //you can write hardware specific statement here to read the IO pin
  {
    return 1;
  }
  return 0;
}

////// this is the function that should be called in a loop with an interval of ~1-6 milliseconds //////
////// everytime encoder is clicked the task will be performed inside this function (check below) //////
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
            //do any action below this line on counter clockwise click
            Serial.println("CCW Click");      //you can write any code here that you want to execute
            //.... 
            high_detected = false;
            low_detected = false;
            mid_detected = false;
          }
        }
        else if((rclk == 0) && (rdt == 1))
        {
          if(high_detected)
          {
            //do any action below this line on clockwise click
            Serial.println("CW Click");      //you can write any code here that you want to execute
            //....
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
