#include "TFClassifier.h"
#include "FashionMNISTParser.h"

#include <gtest/gtest.h>
#include <iostream>

class TestFashionMNIST : public ::testing::Test 
{
protected:
	void SetUp() {}
	void TearDown() {}

	std::string tests_path = 
#if defined(WIN32)
  "../tests";
#else
  "tests";
#endif
};

TEST_F(TestFashionMNIST, test_tensorflow)
{
  FashionMNISTParser parser(tests_path + "/test.csv", 28 * 28);
  TfClassifier tf(tests_path + "/saved_model", 28, 28);

  uint32_t true_predictions = 0;
  uint32_t all_predictions = 0;

  while (true)  
  {
    auto test_content = parser.get_content();
    if(test_content.second.size() == 0) break;
    auto predict_class_type = tf.predict(test_content.second);
    if(predict_class_type == test_content.first) true_predictions++;
    all_predictions++;
  }
  ASSERT_NEAR(static_cast<double>(true_predictions)/all_predictions, 0.91, 1e-2);
}

int main(int argc, char *argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}