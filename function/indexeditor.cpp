#include "indexeditor.h"

#include "function/network/client.h"
#include "function/network/protocal.h"

IndexEditor::IndexEditor(IndexType type, QObject *parent)
    : QObject{parent}, m_type(type), m_org_info{new QJsonObject()}, m_index_info{new QJsonObject()}
{
    m_client = new Client(this);
}

IndexEditor::~IndexEditor()
{
    delete m_org_info;
    delete m_index_info;
}

void IndexEditor::pull_org()
{
    QJsonObject send;
    send["type"] = static_cast<int>(Protocal::HeaderField::REQUEST_TYPEINDEX);
}

void IndexEditor::compressToIndex()
{

}

void IndexEditor::syncToOrg()
{

}

void IndexEditor::push_org()
{

}

void IndexEditor::setOrgData(const QJsonObject &data)
{

}
