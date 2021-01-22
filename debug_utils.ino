void readResponse() {
  int length = 0; // number of characters currently in the buffer
  while(sparthan.available())
      {
        char c = sparthan.read();
        if((c == '\r') || (c == '\n'))
        {
            // end-of-line received
            if(length > 0)
            {
                Serial.print("got response: ");
                Serial.println(buffer);
            }
            length = 0;
        }
        else
        {
            if(length < BUFF_SIZE)
            {
                buffer[length++] = c; // append the received character to the array
                buffer[length] = 0; // append the null terminator
            }
            else
            {
                Serial.println("buffer full"); // buffer full - discard the received character
            }
        }
    } 
}
