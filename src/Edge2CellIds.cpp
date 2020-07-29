#include <liboscar/routing/support/Edge2CellIds.h>

namespace liboscar::routing::support {

std::vector<uint32_t> Edge2CellIds::operator()(GeoPoint const & source, GeoPoint const & tgt) {
    if (m_last.equal(tgt) && !tgt.equal(source)) {
        return (*this)(tgt, source);
    }
    if (!m_last.equal(source)) {
        m_fh = m_tra.grid().faceHint(source);
    }
    std::vector<uint32_t> result;
    m_tra.tds().traverse(
        source,
        tgt,
        [&](auto const & f){
            result.push_back( m_tra.cellIdFromFaceId(f.id()) );
        },
        m_fh,
        m_tra.tds().TT_STRAIGHT
    );
    if (result.size()) {
        m_last = tgt;
        m_fh = result.back();
    }
    std::sort(result.begin(), result.end());
    result.resize(std::distance(result.begin(), std::unique(result.begin(), result.end())));
    return result;
}

} //end namespace liboscar::routing::support