#include <assert.h>

#include <chrono>
#include <ctime>			// std::time_t, std::tm, ...
#include <memory>			// unique_ptr
#include <stack>
#include <type_traits>		// is_base_of
#include <vector>

#include <ndb_init.h>
#include <ndbapi/ndb_cluster_connection.hpp>
#include <ndbapi/Ndb.hpp>
#include <ndbapi/NdbDictionary.hpp>
#include <ndbapi/NdbOperation.hpp>
#include <ndbapi/NdbTransaction.hpp>
#include <portlib/NdbSleep.h>
#include <portlib/NdbThread.h>
#include <util/NdbOut.hpp>
#include <util/OutputStream.hpp>

#include "compat.hpp"
