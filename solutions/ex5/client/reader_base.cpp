#include "reader_base.h"

ReaderBase::ReaderBase(const DescriptorType::executor_type &executor, Client &client, InputPrompt &prompt)
    : m_stdin_desc{executor, ::dup(STDIN_FILENO)}
    , m_client{client}
    , m_prompt{prompt}
{
}

std::string ReaderBase::read_buffer_line()
{
  std::istream is(&m_buffer);
  std::string line;
  std::getline(is, line);
  return line;
}
