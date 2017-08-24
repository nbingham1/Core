/*
 * ascii_stream.h
 *
 *  Created on: Feb 5, 2014
 *      Author: nbingham
 */

#include <std/string.h>
#include <std/stream.h>

#pragma once

namespace core
{

struct ascii_stream
{
	ascii_stream()
	{
		ptr = NULL;
		msg = NULL;
		end = "\n";
		debug = false;
		count = 0;
	}

	ascii_stream(string filename, const char *end = "\n", const char *msg = NULL, bool debug = false)
	{
		ptr = fopen(filename.c_str(), "w");
		this->msg = msg;
		this->debug = debug;
		this->end = end;
		count = 0;
	}

	ascii_stream(const char *filename, const char *end = "\n", const char *msg = NULL, bool debug = false)
	{
		ptr = fopen(filename, "w");
		this->msg = msg;
		this->debug = debug;
		this->end = end;
		count = 0;
	}

	ascii_stream(FILE *ptr, const char *end = "\n", const char *msg = NULL, bool debug = false)
	{
		this->ptr = ptr;
		this->msg = msg;
		this->debug = debug;
		this->end = end;
		count = 0;
	}

	~ascii_stream()
	{
		close();
	}

	FILE *ptr;
	string store;
	const char *msg;
	bool debug;
	int count;
	const char *end;

	void open(const char *filename);
	void close();
	void flush(const char *path, int line);
};

template <typename type>
inline ascii_stream &operator<<(ascii_stream &s1, type s2)
{
	s1.store << s2;
	return s1;
}

template <>
ascii_stream &operator<<(ascii_stream &s1, stream_flush s2);

extern ascii_stream cout;
extern ascii_stream cprog;
extern ascii_stream cnote;
extern ascii_stream cwarn;
extern ascii_stream cerr;
extern ascii_stream cfate;

}

