#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE cassandra
#endif

#include <boost/test/unit_test.hpp>

#include "cassandra.h"
#include "types.hpp"
#include "test_utils.hpp"

struct ConfigTests {
    ConfigTests() { }
};

BOOST_FIXTURE_TEST_SUITE(config, ConfigTests)


BOOST_AUTO_TEST_CASE(test_options)
{
  test_utils::CassClusterPtr cluster = test_utils::make_shared(cass_cluster_new());

  {
    unsigned connect_timeout = 9999;
    cass_cluster_set_connect_timeout(cluster.get(), connect_timeout);
    BOOST_REQUIRE(cluster->config().connect_timeout() == connect_timeout);
  }

  {
    int port = 7000;
    cass_cluster_set_port(cluster.get(), port);
    BOOST_REQUIRE(cluster->config().port() == port);
  }
}

std::string make_contact_point_string(const std::list<std::string>& contact_points) {
  std::string str;

  for (cass::Config::ContactPointList::const_iterator it = contact_points.begin(),
       end = contact_points.end();
       it != end; ++it) {
    if (str.size() > 0) {
      str.push_back(',');
    }
    str.append(*it);
  }
  return str;
}

BOOST_AUTO_TEST_CASE(test_contact_points)
{
  test_utils::CassClusterPtr cluster = test_utils::make_shared(cass_cluster_new());

  // Simple
  const char* contact_points1 = "127.0.0.1,127.0.0.2,127.0.0.3";
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(contact_points1));
  BOOST_REQUIRE(make_contact_point_string(cluster->config().contact_points()).compare(contact_points1) == 0);

  // Clear
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(""));
  BOOST_REQUIRE(cluster->config().contact_points().empty());

  // Extra commas
  const char* contact_points1_commas = ",,,,127.0.0.1,,,,127.0.0.2,127.0.0.3,,,,";
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(contact_points1_commas));
  BOOST_REQUIRE(make_contact_point_string(cluster->config().contact_points()).compare(contact_points1) == 0);

  // Clear
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(""));
  BOOST_REQUIRE(cluster->config().contact_points().empty());

  // Extra whitespace
  const char* contact_points1_ws = "   ,\r\n,  ,   ,  127.0.0.1 ,,,  ,\t127.0.0.2,127.0.0.3,  \t\n, ,,   ";
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(contact_points1_ws));
  BOOST_REQUIRE(make_contact_point_string(cluster->config().contact_points()).compare(contact_points1) == 0);

  // Clear
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(""));
  BOOST_REQUIRE(cluster->config().contact_points().empty());

  // Append
  const char* contact_point1 = "127.0.0.1";
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(contact_point1));

  const char* contact_point2 = "127.0.0.2";
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(contact_point2));

  const char* contact_point3 = "127.0.0.3";
  cass_cluster_set_contact_points(cluster.get(), cass_string_init(contact_point3));

  BOOST_REQUIRE(make_contact_point_string(cluster->config().contact_points()).compare(contact_points1) == 0);
}

BOOST_AUTO_TEST_SUITE_END()


