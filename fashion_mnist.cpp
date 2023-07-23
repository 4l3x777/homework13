#include <iostream>
#include "TFClassifier.h"
#include "FashionMNISTParser.h"

int main(int argc, char * argv[]) {
  if(argc != 3) 
  {
    std::cout << "fashion_mnist [path_to_csv] [path_to_model]"<< std::endl;
    return -1;
  }

  FashionMNISTParser parser(argv[1], 28 * 28);
  TfClassifier tf(argv[2], 28, 28);

  uint32_t true_predicts = 0;
  uint32_t all_predicts = 0;

  while (true) {
    auto content = parser.get_content();
    if(content.second.size() == 0) break;
    auto class_type = tf.predict(content.second);
    if(class_type == content.first) true_predicts++;
    all_predicts++;
  }
  std::cout << "Result true estimate: " << (double)true_predicts/all_predicts << std::endl;
  return 0;
}
