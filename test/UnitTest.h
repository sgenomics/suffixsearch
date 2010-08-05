#ifndef UNITTEST_H
#define UNITTEST_H

#include <iostream>
#include <sstream>
#include "stringify.h"

#define teste(t1,t2) _test(t1,t2,__LINE__,__FILE__)
#define test_gt(t1,t2) _test_gt(t1,t2,__LINE__,__FILE__)
#define test_equality(t1,t2) _test(t1,t2,__LINE__,__FILE__)
#define test_truth(t1) _test(t1,__LINE__,__FILE__)
#define test_approx(t1,t2,t3) _test_approx(t1,t2,t3,__LINE__,__FILE__)

class UnitTest {
private:
  size_t tests_failed;
  size_t tests_passed;
  size_t total_tests_failed;
  size_t total_tests_passed;
  size_t test_id;
  std::string current_description;
  std::string test_set_name;

  std::ostringstream tap;
public:

  UnitTest(std::string test_set_name_in) : tests_failed(0),
                                           tests_passed(0),
                                           total_tests_failed(0),
                                           total_tests_passed(0),
                                           test_id(0),
                                           current_description(),
                                           test_set_name(test_set_name_in) {
    std::cout << "*** Test set  : " << test_set_name << std::endl;
  }

  void begin_test_set(std::string description) {
    current_description = description;
    tests_failed = 0;
    tests_passed = 0;
    std::cout << "****** Testing: " << current_description << std::endl;
  }

  void end_test_set() {
    std::cout << "****** Test   : " << current_description << " complete, ";
    std::cout << "passed " << tests_passed;
    std::cout << ", failed " << tests_failed << "." << std::endl;
  }

  void _test(bool t1,int linenumber,const char *current_file) {
    if(!t1) {
      std::cout << "****** FAILED : " << current_file << "," << linenumber << std::endl;
      tap << "not ok " << test_id << " at: " << current_file << "," << linenumber << std::endl;
      total_tests_failed++;
      tests_failed++;
    } else { 
      tests_passed++;
      total_tests_passed++; 
      tap << "ok " << test_id << " at: " << current_file << "," << linenumber << std::endl;
    }
    test_id++;
  }

  template<class _TestType>
  void _test(_TestType t1,_TestType t2,int linenumber,const char *current_file) {
    bool test_result = (t1 == t2);

    if(!test_result) {
        std::cout << "****** FAILED : " << current_file << "," << linenumber;
	std::cout << ": " << t1 << " is not equal to " << t2 << std::endl;
        tap << "not ok " << test_id << " at: " << current_file << "," << linenumber << "(" << t1 << " not equal to " << t2 << ")" << std::endl;
        total_tests_failed++;
        tests_failed++;
    } else { 
      tests_passed++; 
      total_tests_passed++; 
      tap << "ok " << test_id << " at: " << current_file << "," << linenumber << std::endl;
    }
    test_id++;
  }
  
  template<class _TestType>
  void _test_gt(_TestType t1,_TestType t2,int linenumber,const char *current_file) {
    bool test_result = (t1 > t2);

    if(!test_result) {
        std::cout << "****** FAILED : " << current_file << "," << linenumber;
	std::cout << ": " << t1 << " is not greater than " << t2 << std::endl;
        tap << "not ok " << test_id << " at: " << current_file << "," << linenumber << " (" << t1 << " not greater than " << t2 << ")" << std::endl;
        total_tests_failed++;
        tests_failed++;
    } else { 
      tests_passed++; 
      total_tests_passed++; 
      tap << "ok " << test_id << " at: " << current_file << "," << linenumber << std::endl;
    }

    test_id++;
  }

  template<class _TestType>
  void _test_approx(_TestType t1,_TestType t2,_TestType max_error,int linenumber,const char *current_file) {
    _TestType difference = t1-t2;
    if(difference < 0) difference = 0-difference;

    if(difference > max_error) {
      std::cout << "****** FAILED : " << current_file << "," << linenumber;
      std::cout << ": " << t1 << " is not equal to " << t2;
      std::cout << " within difference of " << max_error << std::endl;
      tap << "not ok " << test_id << " at: " << current_file << "," << linenumber << " (" << t1 << " not equal to " << t2 << " within: " << max_error << std::endl;
      tests_failed++;
      total_tests_failed++;
    } else { 
      tests_passed++;
      total_tests_passed++;
      tap << "ok " << test_id << " at: " << current_file << "," << linenumber << std::endl;
    }
  }

  void test_report() {
    std::cout << "*** Test set  : " << test_set_name << " complete, ";
    std::cout << "passed " << total_tests_passed;
    std::cout << " failed " << total_tests_failed << "." << std::endl;
    if(total_tests_failed != 0) std::cout << "*** TEST FAILED!" << std::endl;
  }

  std::string tap_report() {
    return std::string("1..") + stringify(test_id-1) + "\n" + tap.str();
  }

};

#endif
