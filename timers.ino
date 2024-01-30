#if defined(ESP32)

void timersStart(){
  const esp_timer_create_args_t timer_args1 = {
    .callback = &timer_callback1, //link the call back
    .arg = nullptr, //not passing in anything
    .dispatch_method = ESP_TIMER_TASK, //The handler will be called from the task (another option is from the ISR)
    .name = "timer1", //nice name
    .skip_unhandled_events = false     //If any timer operation was missed, then process it anyway
  };
  //create timer, not running yet..
  ESP_ERROR_CHECK(esp_timer_create(&timer_args1, &timer1));
}

void timersData(){

  if (esp_timer_is_active(timer1)) {
    timerActive1 = true;
  }else{
    timerActive1 = false;
  }

    if (!timerActive1) {
    //check state of timer and stop it if needed..
    if (esp_timer_is_active(timer1)) {
      ESP_ERROR_CHECK(esp_timer_stop(timer1));
    }
    //start our one shot timer for 1 second.. periodic
    ESP_ERROR_CHECK(esp_timer_start_periodic(timer1, 2*1000*1000));
    timerActive1 = true;
  }

}

#endif