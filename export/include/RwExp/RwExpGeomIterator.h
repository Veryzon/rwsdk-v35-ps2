#ifndef _RWEXPGEOMITEROATOR
#define _RWEXPGEOMITEROATOR

#include "RwCommMesh.h"

template<class Geometry, class Data, class DataFunc>
class RwExpGeomIterator  
{
public:
    RwExpGeomIterator() :  m_iter(0)
    {
    }

    RwCommError     ResetPrimIterator(Geometry *pGeometry)
    {
        m_vData.clear();
        if (pGeometry)
        {
            RwCommError res = pGeometry->ResetPrimIterator();
            while (res == RWCOMM_SUCCESS)
            {   
                Data data;
                DataFunc dataFunc;
            
                dataFunc(pGeometry, data);
                m_vData.push_back(data);
                res = pGeometry->AdvancePrimIterator();
            }
        }

        m_iter = 0;
        return (m_iter < m_vData.size() ? RWCOMM_SUCCESS : RWCOMM_ITERATOR_END);
    }

    RwCommError     AdvancePrimIterator()
    {
        m_iter++;
        return (m_iter < m_vData.size() ? RWCOMM_SUCCESS : RWCOMM_ITERATOR_END);
    }

    RwCommError     GetData(Data &data, vector<int> &vDataMap) const
    {
        data = m_vData[ vDataMap[m_iter] ];
        return RWCOMM_SUCCESS;
    }


private:
    int                 m_iter;
    vector<Data>        m_vData;
};

// Triangle extractor
struct RwExpGetTriangulation
{
    void operator() (RwCommMesh *pMesh, vector<int> &vTriangulation)
    {
        pMesh->GetPrimTriangulation(vTriangulation);
    }
};


typedef RwExpGeomIterator<RwCommMesh, vector<int> , RwExpGetTriangulation>  RwExpTriangulationExtractor;

#endif