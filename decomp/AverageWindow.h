class AverageBase {
public:
	void *operator new(std::size_t size) {
		return gFastMem.Alloc(size, nullptr);
	}

	void operator delete(void *mem, std::size_t size) {
		if (mem) {
			gFastMem.Free(mem, size, nullptr);
		}
	}

	AverageBase(int size, int slots)
			: nSize(size),   //
			  nSlots(slots), //
			  nSamples(0),   //
			  nCurrentSlot(0) {}

	virtual ~AverageBase() {}

	void *Allocate(unsigned int size, const char *name) {
		return gFastMem.Alloc(size, name);
	}
	void DeAllocate(void *ptr, unsigned int size, const char *name) {
		return gFastMem.Free(ptr, size, name);
	}

	unsigned char GetNumSamples() {
		return nSamples;
	}

	virtual void Recalculate() {}

protected:
	unsigned char nSize;
	unsigned char nSlots;
	unsigned char nSamples;
	unsigned char nCurrentSlot;
};

class Average : public AverageBase {
public:
	Average()
			: AverageBase(4, 0), //
			  fAverage(0.0f),    //
			  pData(NULL),       //
			  fTotal(0.0f) {}

	Average(int slots)
			: AverageBase(4, slots), //
			  fAverage(0.0f),        //
			  pData(NULL),           //
			  fTotal(0.0f) {
		Init(slots);
	}

	void Init(int slots) {
		if (pData && pData != SmallDataBuffer) {
			gFastMem.Free(pData, 4*nSlots, "Average::pData");
			pData = nullptr;
		}
		nSlots = slots;
		pData = SmallDataBuffer;
		if (slots > 5) {
			pData = (float*)gFastMem.Alloc(4*slots, "Average::pData");
		}
		memset(pData, 0, 4 * slots);
	}

	float GetValue() {
		return fAverage;
	}

	float GetTotal() {
		return fTotal;
	}

protected:
	float fTotal;
	float fAverage;
	float *pData;

private:
	float SmallDataBuffer[5];
};

class AverageWindow : public Average {
public:
	AverageWindow(float f_timewindow, float f_frequency)
			: Average(f_timewindow * f_frequency + 0.5f), //
			  fTimeWindow(f_timewindow),                  //
			  iOldestValue(0),                            //
			  AllocSize(4 * nSize) {
		pTimeData = (float*)Allocate(AllocSize, "AverageWindow::TimeData");
		memset(pTimeData, 0, AllocSize);
	}

	float GetOldestValue() {
		return pData[iOldestValue];
	}

	float GetOldestTimeValue() {
		return pTimeData[iOldestValue];
	}

	// todo this wasn't in the decomp, verify
	void Record(const float fValue, const float fTimeNow) {
		if (pData[nCurrentSlot] == 0.0 && pTimeData[nCurrentSlot] == 0.0) {
			nSamples++;
		}
		else {
			fTotal -= pData[nCurrentSlot];
		}
		fTotal += fValue;
		pData[nCurrentSlot] = fValue;
		pTimeData[nCurrentSlot] = fTimeNow;

		while (fTimeNow - pTimeData[iOldestValue] > fTimeWindow) {
			if (pTimeData[iOldestValue] > 0.0) {
				fTotal -= pData[iOldestValue];
				pData[iOldestValue] = 0.0;
				pTimeData[iOldestValue] = 0.0;
				nSamples--;
			}
			if (++iOldestValue >= nSlots) {
				iOldestValue = 0;
			}
		}

		auto avg = fTotal;
		if (nSamples) {
			avg = fTotal / (double)nSamples;
		}
		fAverage = avg;
		if (++nCurrentSlot >= nSlots) {
			nCurrentSlot = 0;
		}
	}
	void Reset(float fValue) {
		for (int i = 0; i < nSlots; i++) {
			pData[i] = fValue;
			pTimeData[i] = 0;
		}
		fAverage = 0;
		nSamples = 0;
		iOldestValue = 0;
		nCurrentSlot = 0;
		fTotal = nSlots * fValue;
	}

	float fTimeWindow;
	int iOldestValue;
	float *pTimeData;
	unsigned int AllocSize;
};

class Graph {
public:
	Graph(bVector2 *points, int num_points) {
		Points = points;
		NumPoints = num_points;
	}

	float GetValue(float x) {
		// todo this is entirely guessed, it wasn't in decomp and its pseudocode confusing as hell
		auto curve = Points[0].x;
		for (int i = 0; i < NumPoints; i++) {
			if (x < Points[i].x) break;

			if (i >= NumPoints-1) return curve = Points[i].y;

			float delta = x;
			delta -= Points[i].x;
			delta /= (Points[i+1].x - Points[i].x);
			curve = std::lerp(Points[i].y, Points[i+1].y, delta);
		}
		return curve;
	}

private:
	bVector2 *Points; // offset 0x0, size 0x4
	int NumPoints;    // offset 0x4, size 0x4
};

template <typename T> struct GraphEntry {
	T x;
	T y;
};

template <typename T> class tGraph {
public:
	tGraph(GraphEntry<T> *data, int num) {
		GraphData = data;
		NumEntries = num;
	}
	void Blend(T *dest, T *a, T *b, const T blend_a);

	// Credits: Brawltendo
	// UNSOLVED
	void GetValue(T *pValue, T x) {
		if (NumEntries > 1) {
			if (x <= GraphData[0].x) {
				memcpy(pValue, &GraphData[0].y, sizeof(float));
			} else if (x >= GraphData[NumEntries - 1].x) {
				memcpy(pValue, &GraphData[NumEntries - 1].y, sizeof(float));
			} else {
				for (int i = 0; i < NumEntries - 1; ++i) {
					if (x >= GraphData[i].x && x < GraphData[i + 1].x) {
						const T blend = (x - GraphData[i].x) / (GraphData[i + 1].x - GraphData[i].x);
						Blend(pValue, &GraphData[i + 1].y, &GraphData[i].y, blend);
						return;
					}
				}
			}
		} else if (NumEntries > 0) {
			memcpy(pValue, &GraphData[0].y, sizeof(float));
		}
	}

	float GetValue(T x) {
		float ret;
		GetValue(&ret, x);
		return ret;
	}

private:
	GraphEntry<T> *GraphData; // offset 0x0, size 0x4
	int NumEntries;           // offset 0x4, size 0x4
};

template <> void tGraph<float>::Blend(float *dest, float *a, float *b, const float blend_a) {
	*dest = *a * blend_a + *b * (1.0f - blend_a);
}