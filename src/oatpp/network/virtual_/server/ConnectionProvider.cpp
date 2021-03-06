/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#include "ConnectionProvider.hpp"

namespace oatpp { namespace network { namespace virtual_ { namespace server {

ConnectionProvider::ConnectionProvider(const std::shared_ptr<virtual_::Interface>& interface)
  : m_interface(interface)
  , m_open(true)
  , m_maxAvailableToRead(-1)
  , m_maxAvailableToWrite(-1)
{
  setProperty(PROPERTY_HOST, m_interface->getName());
  setProperty(PROPERTY_PORT, "0");
}

std::shared_ptr<ConnectionProvider> ConnectionProvider::createShared(const std::shared_ptr<virtual_::Interface>& interface) {
  return std::make_shared<ConnectionProvider>(interface);
}

void ConnectionProvider::setSocketMaxAvailableToReadWrtie(data::v_io_size maxToRead, data::v_io_size maxToWrite) {
  m_maxAvailableToRead = maxToRead;
  m_maxAvailableToWrite = maxToWrite;
}

void ConnectionProvider::close() {
  m_open = false;
  m_interface->notifyAcceptors();
}

std::shared_ptr<ConnectionProvider::IOStream> ConnectionProvider::getConnection() {
  auto socket = m_interface->accept(m_open);
  if(socket) {
    socket->setMaxAvailableToReadWrtie(m_maxAvailableToRead, m_maxAvailableToWrite);
  }
  return socket;
}

}}}}
