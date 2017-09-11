COV = gcov
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -static -lgcov --coverage
