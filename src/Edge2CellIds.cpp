#include <liboscar/routing/support/Edge2CellIds.h>

namespace liboscar::routing::support {


//use hint as starting triangle for source, changes it to one triangle containing tgt
std::vector<uint32_t> Edge2CellIds::operator()(GeoPoint const & source, GeoPoint const & tgt, Hint & hint) {
	if (hint == m_tra.tds().NullFace) {
		hint = m_tra.grid().faceHint(source);
	}
	std::vector<uint32_t> result;
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
