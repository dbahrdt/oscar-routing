#include <liboscar/routing/support/Edge2CellIds.h>

namespace liboscar::routing::support {


//use hint as starting triangle for source, changes it to one triangle containing tgt
std::vector<uint32_t> Edge2CellIds::operator()(GeoPoint const & source, GeoPoint const & tgt, Hint & hint) {
	if (hint == m_tra.tds().NullFace) {
		hint = m_tra.grid().faceHint(source);
	}
	std::vector<uint32_t> result;
	auto traverse = [this, &result](GeoPoint const & source, GeoPoint const & tgt, Hint hint) -> Hint {
		hint = m_tra.tds().traverse(
			source,
			tgt,
			[&](auto const & f){
				auto cid = m_tra.cellIdFromFaceId(f.id());
				if (cid != m_tra.NullCellId) {
					result.push_back(cid);
				}
			},
			hint,
			m_tra.tds().TT_STRAIGHT
		);
		std::sort(result.begin(), result.end());
		result.resize(std::distance(result.begin(), std::unique(result.begin(), result.end())));
		return hint;
	};
	
	if (hint == m_tra.tds().NullFace) {//this means that source is outside of our triangulation
		//compute it the other way round iff hint for tgt is ok
		hint = m_tra.grid().faceHint(tgt);
		if (hint == m_tra.tds().NullFace) { //both are outside, triangulation does not support this, though there might be cells intersecting this edge
			return std::vector<uint32_t>();
		}
		//we compute the exact start triang here since we need it anyway
		hint = m_tra.tds().locate(tgt, hint);
		traverse(tgt, source, hint);
	}
	else {
		hint = traverse(source, tgt, hint);
	}
	return result;
}

std::vector<uint32_t> Edge2CellIds::operator()(GeoPoint const & source, GeoPoint const & tgt) {
	if (m_last.equal(tgt) && !tgt.equal(source)) {
		return (*this)(tgt, source);
	}
	if (!m_last.equal(source)) {
		m_fh = m_tra.grid().faceHint(source);
	}
	return (*this)(source, tgt, m_fh);
}

} //end namespace liboscar::routing::support
