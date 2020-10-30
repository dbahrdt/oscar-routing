#pragma once
#ifndef OSCAR_PATH_FINDER_EDGE_TO_OSCAR_CELLIDS_H
#define OSCAR_PATH_FINDER_EDGE_TO_OSCAR_CELLIDS_H
#include <liboscar/OsmKeyValueObjectStore.h>


namespace liboscar::routing::support {

class Edge2CellIds final {
public:
    using GeoPoint = sserialize::spatial::GeoPoint;
    using TriangulationGeoHierarchyArrangement = sserialize::Static::spatial::TriangulationGeoHierarchyArrangement;
    using Hint = TriangulationGeoHierarchyArrangement::Triangulation::FaceId;
public:
    Edge2CellIds(liboscar::Static::OsmKeyValueObjectStore const & store) : m_tra(store.regionArrangement()) {}
    Edge2CellIds(const Edge2CellIds &) = default;
    ~Edge2CellIds() {}
    Edge2CellIds & operator=(Edge2CellIds const&) = default;
public:
    template<typename T_OUTPUT_ITERATOR>
    void operator()(GeoPoint const & source, GeoPoint const & tgt, T_OUTPUT_ITERATOR out) {
        auto tmp = (*this)(source, tgt);
        std::copy(tmp.begin(), tmp.end(), out);
    }
    template<typename T_OUTPUT_ITERATOR>
    void operator()(double lat_source, double lon_source, double lat_tgt, double lon_tgt, T_OUTPUT_ITERATOR out) {
        (*this)(GeoPoint(lat_source, lon_source), GeoPoint(lat_tgt, lon_tgt), out);
    }
public:
    inline std::vector<uint32_t> operator()(double lat_source, double lon_source, double lat_tgt, double lon_tgt) {
        return (*this)(GeoPoint(lat_source, lon_source), GeoPoint(lat_tgt, lon_tgt));
    }
    std::vector<uint32_t> operator()(GeoPoint const & source, GeoPoint const & tgt);

    inline std::vector<uint32_t> operator()(double lat_source, double lon_source, double lat_tgt, double lon_tgt, Hint & hint) {
      return (*this)(GeoPoint(lat_source, lon_source), GeoPoint(lat_tgt, lon_tgt), hint);
    }
    //use hint as starting triangle for source, changes it to one triangle containing tgt
    std::vector<uint32_t> operator()(GeoPoint const & source, GeoPoint const & tgt, Hint & hint);
private:
    GeoPoint m_last;
    Hint m_fh{0};
    sserialize::Static::spatial::TriangulationGeoHierarchyArrangement m_tra;
};

//Definitions



}//end namespace liboscar::routing


#endif
