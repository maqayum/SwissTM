/**
 * Simple profiling of transaction duration and some performance counters.
 * The goal of this profiler is to gather more precise information about
 * transaction duration and performance counters without overheads of
 * collecting precise information about reads, writes, etc.
 *
 * @author Aleksandar Dragojevic aleksandar.dragojevic@epfl.ch
 *
 */

#ifndef WLPDSTM_PROFILING_NON_TX_DURATION_PCNT_SIMPLE_H_
#define WLPDSTM_PROFILING_NON_TX_DURATION_PCNT_SIMPLE_H_

#include <stdint.h>

#include "../word.h"
#include "../timing.h"

#include "../perfcnt/perf_cnt_impl.h"

namespace wlpdstm {

	template<class STATS, class STATS_TYPE>
	class NonTxDurationPcntProfilingSimple {
		public:
			static void GlobalInit() {
				PerfCntImpl::GlobalInit();
			}
			
			void ThreadInit(STATS *stats_);
			void ThreadShutdown();

			/////////////////////
			// callbacks start //
			/////////////////////

			void ThreadStart();
			void ThreadEnd();

			void TxStartStart(int lex_tx_id);
			void TxStartEnd();

			void TxCommitStart();
			void TxCommitEnd();

			void ReadWordStart(Word *addr);
			void ReadWordEnd(Word res);

			void WriteWordStart(Word *addr, Word val, Word mask = LOG_ENTRY_UNMASKED);
			void WriteWordEnd();

			void TxMallocStart(size_t size);
			void TxMallocEnd(void *ptr);

			void TxFreeStart(void *ptr, size_t size);
			void TxFreeEnd();
				
			void TxRestartStart();
			void TxRestartEnd();
				
			void TxAbortStart();			
			void TxAbortEnd();
			
			///////////////////
			// callbacks end //
			///////////////////

		protected:
			void RecordDuration();

		protected:
			STATS *stats;

//			uint64_t tx_start_cycles;

			enum LastTransactionState {
				NO_TX,
				COMMITTED,
				RESTARTED
			};

			LastTransactionState last_tx_state;
            PerfCntImpl perf_cnt;
	};
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::ThreadInit(STATS *stats_) {
	stats = stats_;
	last_tx_state = NO_TX;
    perf_cnt.Init();
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::ThreadShutdown() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::RecordDuration() {
	if(last_tx_state == COMMITTED) {
        perf_cnt.End();
		stats->IncrementStatisticsPerTxType(STATS_TYPE::TX_DURATION_COMMITTED, perf_cnt.GetElapsedCycles());
		stats->IncrementStatisticsPerTxType(STATS_TYPE::TX_PROFILED_COMMITS);
	} else if(last_tx_state == RESTARTED) {
        perf_cnt.End();
		stats->IncrementStatisticsPerTxType(STATS_TYPE::TX_DURATION_RESTARTED, perf_cnt.GetElapsedCycles());
		stats->IncrementStatisticsPerTxType(STATS_TYPE::TX_PROFILED_RESTARTS);
	} else {
        perf_cnt.Start();
    }

	last_tx_state = COMMITTED;
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxStartStart(int lex_tx_id) {
	RecordDuration();
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxStartEnd() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxCommitStart() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxCommitEnd() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::ReadWordStart(Word *addr) {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::ReadWordEnd(Word res) {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::WriteWordStart(Word *addr, Word val, Word mask) {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::WriteWordEnd() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxMallocStart(size_t size) {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxMallocEnd(void *ptr) {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxFreeStart(void *ptr, size_t size) {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxFreeEnd() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxRestartStart() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxRestartEnd() {
	last_tx_state = RESTARTED;
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxAbortStart() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::TxAbortEnd() {
	// empty
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::ThreadStart() {
	RecordDuration();
}

template<class STATS, class STATS_TYPE>
inline void wlpdstm::NonTxDurationPcntProfilingSimple<STATS, STATS_TYPE>::ThreadEnd() {
	RecordDuration();
}

#endif /* WLPDSTM_PROFILING_NON_TX_DURATION_PCNT_SIMPLE_H_ */
