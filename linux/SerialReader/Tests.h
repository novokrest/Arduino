#ifndef TESTS_H
#define TESTS_H


class Tests
{
    Tests() = delete;
    Tests(const Tests&) = delete;
    Tests& operator=(const Tests&) = delete;

    static void KeyboardNotificationTest();

public:
    static void RunTests();
};

#endif // TESTS_H
