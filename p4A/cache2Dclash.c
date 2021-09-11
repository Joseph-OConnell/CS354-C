// Name: Joseph O'Connell
int arr[128][8];

int main() {
   for (int i = 0; i < 100; i++){
      for (int row = 0; row < 128; row += 64){
	 for (int col = 0; col < 8; col++){
	    arr[row][col] = i + row + col;
	 }
      }
   }
  return 0;
}
