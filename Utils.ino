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

void getIntCmd(String sCmd) {
	for (int i = 0; i < 5; ++i) {
	}
}

void parcCmdFromApp(String sCmd) {
	String s = sCmd.substring(1, sCmd.length() - 1);
	
	char* p = (char*)malloc(64);
	s.toCharArray(p, 64);
	char* str;
	int i = 0;
	while ((str = strtok_r(p, ",", &p)) != NULL) {
		parsedCmd[i] = ((String)str).toFloat();
		i++;
		if (i > 4) {
			return;
		}
	}
	free(p);
}

float* getFloatCmd(int *intCmd) {
	float* floatCmd = (float*)malloc(5 * sizeof(float)); 
	
}