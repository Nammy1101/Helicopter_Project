#include "cxxtest/TestSuite.h"

class MyTestSuite : public CxxTest::TestSuite 


void testMultiplication( void )
{
TS_ASSERT_EQUALS( 2 * 2, 5 );
}
};