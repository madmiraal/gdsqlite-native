#ifndef GDSQLITE_H
#define GDSQLITE_H

#include <Godot.hpp>
#include <Reference.hpp>

#include <File.hpp>

// SQLite3
#include <sqleet.h>
#include <spmemvfs.h>

#ifndef NULL
#define NULL 0
#endif

namespace godot {
	class SQLite : public Reference {
		GODOT_CLASS(SQLite, Reference)

	private:
		// sqlite handler
		sqlite3 *db;

		// vfs
		spmemvfs_db_t p_db;
		bool memory_read;

		const int ALLOWED_FLAGS_MASK = SQLITE_OPEN_READONLY |
		SQLITE_OPEN_READWRITE |
		SQLITE_OPEN_CREATE |
		SQLITE_OPEN_URI |
		SQLITE_OPEN_MEMORY |
		SQLITE_OPEN_NOMUTEX |
		SQLITE_OPEN_FULLMUTEX |
		SQLITE_OPEN_SHAREDCACHE |
		SQLITE_OPEN_PRIVATECACHE |
		SQLITE_OPEN_NOFOLLOW;

	public:
		const int DEFAULT_OPEN_FLAGS = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;

		enum {
			RESULT_BOTH = 0,
			RESULT_NUM,
			RESULT_ASSOC
		};

		static void _register_methods();

		// constructor
		SQLite();
		~SQLite();
		void _init() {}

		// methods
		bool open(String path);
		bool open_with_flags(String path, int flags);
		bool open_buffered(String name, PoolByteArray buffers, int64_t size);
		bool open_encrypted(String path, String password);
		bool open_encrypted_with_flags(String path, String password, int flags);
		void close();

		bool query(String statement);
		bool query_with_args(String statement, Array args);
		bool query_all(String statement_list);
		Array fetch_array(String statement);
		Array fetch_array_with_args(String statement, Array args);
		Array fetch_assoc(String statement);
		Array fetch_assoc_with_args(String statement, Array args);

	private:
		sqlite3_stmt* prepare(const char* statement);
		Array fetch_rows(String query, Array args, int result_type = RESULT_BOTH);
		Dictionary parse_row(sqlite3_stmt *stmt, int result_type);
		sqlite3* get_handler() { return (memory_read ? p_db.handle : db); }
		bool bind_args(sqlite3_stmt *stmt, Array args);
	};
}
#endif
