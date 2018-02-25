#include "cmasternode.h"
#include <QBuffer>

/*
CMasternode& CMasternode::operator =(const CMasternode &other)
{
    if (this == &other)
    {
        return *this;
    }
}
*/

QDataStream& operator>>(QDataStream& in, CMasternode& data)
{
    in>>data.m_num>>data.m_alias>>data.m_ip>>
            data.m_port>>data.m_status>>data.m_user>>
            data.m_pwd>>data.m_remote_rpc_user>>data.m_remote_rpc_pwd>>
            data.m_remote_rpc_ip>>data.m_local_rpc_user>>
            data.m_local_rpc_pwd>>data.m_local_rpc_ip>>
            data.m_mn_key>>data.m_clltrl_hash>>data.m_index>>
            data.m_safe_conf_path>>data.m_mn_conf_path>>data.m_mn_step;
     return in;
}

QDataStream& operator<<(QDataStream& out, const CMasternode& data)
{
    out<<data.m_num<<data.m_alias<<data.m_ip<<
            data.m_port<<data.m_status<<data.m_user<<
            data.m_pwd<<data.m_remote_rpc_user<<data.m_remote_rpc_pwd<<
            data.m_remote_rpc_ip<<data.m_local_rpc_user<<
            data.m_local_rpc_pwd<<data.m_local_rpc_ip<<
            data.m_mn_key<<data.m_clltrl_hash<<data.m_index<<
            data.m_safe_conf_path<<data.m_mn_conf_path<<data.m_mn_step;
     return out;
}

CMasternode CMasternode::DeSerializable(const QByteArray &datagram)
{
    QByteArray tmp_array = datagram;
    QBuffer buffer(&tmp_array);
    buffer.open(QIODevice::ReadOnly);

    QDataStream in(&buffer);
    CMasternode cmn;
    in>>cmn;
    buffer.close();
    return cmn;
}

QByteArray CMasternode::Serializable(const CMasternode &cmn)
{
    QByteArray tmp_array;
    QBuffer buffer(&tmp_array);
    buffer.open(QIODevice::WriteOnly);

    QDataStream out(&buffer);
    out<<cmn;
    buffer.close();
    return tmp_array;
}

