String array2string(float* a, int n) {
  String out = "";   
        for (int i = 0; i < n; i++){
          out += (String) *(a + i);
          out += " ";
  }
  return out;
}

String array2string(int* a, int n) {
	String out = "[";   
	for (int i = 0; i < n; i++){
		out += (String) *(a + i);
		if (i != n - 1) {
			out += ", ";
		}
	}
	out += "]";
	return out;
}

int* getIntCmd(String sCmd) {
	for (int i = 0; i < 5; ++i) {
		intCmd[i] = ((String)sCmd.charAt(i)).toInt();
	}
}

float* getFloatCmd(int *intCmd) {
	float* floatCmd = (float*)malloc(5 * sizeof(float)); 
	/* watch closely */
	floatCmd[0] = intCmd[1] * getFingerFBPos(1);
	floatCmd[1] = intCmd[3] * getFingerFBPos(3);
	floatCmd[2] = intCmd[2] * getFingerFBPos(2);
	floatCmd[3] = intCmd[4] * getFingerFBPos(4);
	floatCmd[4] = intCmd[0] * getFingerFBPos(0);

	
}