// Name: Joseph O'Connell
int arr[3000][500];

int main() {
   for (int i = 0; i < 3000; i++){
      for (int j = 0; j < 500; j++){
         arr[i][j] = i + j;
      }
   }
  return 0;
}
