#include <stdio.h>
#include <fstream.h>

#include "libdbg/dbg.h"

p_map::p_map(const char *name, bool allow_subkeys)
{
  _name = strdup(name);
  _allow_subkeys = allow_subkeys;
}

p_map::~p_map(void)
{
  if (_name)
    free(_name);
}

const char *
p_map::get_name(void)
{
  if (_name)
    return _name;
  return "<none>";
}

void
p_map::set_allow_subkeys(bool allow_subkeys)
{
  _allow_subkeys = allow_subkeys;
}

bool
p_map::get_allow_subkeys(void)
{
  return _allow_subkeys;
}

p_tree::p_tree(void)
{
  _map = new p_map("", false);
}

p_tree::~p_tree(void)
{
  delete _map;
}

void
p_tree::add(const char *data, bool allow_subkeys)
{
  p_map *m;
  char *ptr, *tmp;
  char d[1024];

  strncpy(d, data, 1024);
  d[1023] = '\0';

  m = _map;
  ptr = strtok(d, "/");
  while (ptr != 0)
    {
      p_map::const_iterator m_it = m->find(ptr);
      if (m_it == m->end())
        {
	  tmp = strdup(ptr);
          p_map *new_map = new p_map(tmp, false);
          (*m)[tmp] = new_map;
          m = new_map;
        }
      else
        {
          m = (*m_it).second;
        }
      ptr = strtok(0, "/");
    }
  if (allow_subkeys)
    m->set_allow_subkeys(true);
}

bool
p_tree::check_path(const char *data)
{
  p_map *m;
  char *ptr;
  char d[1024];

  strncpy(d, data, 1024);
  d[1023] = '\0';

  m = _map;
  ptr = strtok(d, "/");
  while (ptr != 0)
    {
      if (m->get_allow_subkeys())
        break;
      p_map::const_iterator m_it = m->find(ptr);
      if (m_it == m->end())
        {
          return false;
        }
      m = (*m_it).second;
      ptr = strtok(0, "/");
    }
  
  return true;
}

void
p_tree::dump(ostream& os, p_map *map, int level) const
{
  for (int a = 0;a < level;a++)
    os << "  ";
  os << map->get_name();
  if (map->get_allow_subkeys())
    os << " <*>";
  os << endl;

  for (p_map::iterator it = map->begin();it != map->end();it++)
    dump(os, (*it).second, level + 1);
}

ostream& operator<< (ostream& os, const p_tree& t)
{
  os << "p_tree:" << endl;
  t.dump(os, t._map, 0);
  return os;
}

DBG_class *DBG_class::_singleton = 0;

DBG_class::DBG_class(void)
{
  char *filename, *tmp;
  
  tmp = getenv("HOME");
  if (tmp)
    {
      filename = new char[strlen(tmp) + 8];
      strcpy(filename, tmp);
      strcat(filename, "/.debug");
      load_file(filename);
      delete filename;
    }
  
  _o = &cout;
}

DBG_class::~DBG_class(void)
{
}

DBG_class *
DBG_class::instance(void)
{
  if (_singleton == 0)
    _singleton = new DBG_class();
  return _singleton;
}

void
DBG_class::load_file(const char *filename)
{
  ifstream is;
  char line[4096];

  is.open(filename);
  if (!is)
    return;

  while (242)
    {
      is.getline(line, 4096);
      if (is.eof())
	break;
      if (line[0] != '#')
        add_path(line, false);
    }

  is.close();
}

void
DBG_class::add_path(const char *path, bool allow_subkeys)
{
  _tree.add(path, allow_subkeys);
}

void
DBG_class::form(const char *path, const char *format ...)
{
  char buf[8192];

  if (!_tree.check_path(path))
    return;

  va_list ap;
  va_start(ap, format);
  vsnprintf(buf, 8192, format, ap);
  *_o << buf;
  va_end(ap);
}

void
DBG_class::print(const char *path, const char *msg)
{
  if (!_tree.check_path(path))
    return;

  *_o << "DEBUG: " << path << " - " << msg;
}

void
DBG_class::println(const char *path, const char *msg)
{
  if (!_tree.check_path(path))
    return;

  *_o << "DEBUG: " << path << " - " << msg << endl;
}

bool
DBG_class::check(const char *path)
{
  return _tree.check_path(path);
}

ostream& operator<< (ostream& os, const DBG_class& dbg)
{
  os << "DBG:" << endl;
  os << dbg._tree;
  return os;
}
