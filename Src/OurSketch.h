#ifndef OURSKETCH_H
#define OURSKETCH_H

#include "Abstract.h"
#include <limits> 

#define COUNTER_PER_BUCKET 4

template<typename DATA_TYPE>
class OurSketch : public Abstract<DATA_TYPE> {
public:
    typedef std::unordered_map<DATA_TYPE, COUNT_TYPE> HashMap;

    struct Bucket{
        DATA_TYPE ID[COUNTER_PER_BUCKET];
        COUNT_TYPE count[COUNTER_PER_BUCKET];
        // COUNT_TYPE stability[COUNTER_PER_BUCKET];

        COUNT_TYPE Query(const DATA_TYPE item) {
            for(uint32_t i = 0; i < COUNTER_PER_BUCKET; i++) {
                if(ID[i] == item) {
                    return count[i];
                }
            }
            return 0;
        }
    };

	OurSketch(uint32_t _MEMORY, uint32_t _STAGE1_BIAS = 0, std::string _name = "OurSketch"){
	    this->name = _name;

        this->stage1_bias = _STAGE1_BIAS;
	    LENGTH = _MEMORY / sizeof(Bucket);

        buckets = new Bucket[LENGTH];

        memset(buckets, 0, sizeof(Bucket) * LENGTH);
	}

	~OurSketch(){
        delete [] buckets;
	}

	void Insert(const DATA_TYPE& item) {
        uint32_t pos = hash(item) % LENGTH;
        int minIndex = -1;
        COUNT_TYPE minVal = std::numeric_limits<COUNT_TYPE>::max();

        for (uint32_t i = 0; i < COUNTER_PER_BUCKET; i++){
            if(buckets[pos].ID[i] == item){
                buckets[pos].count[i]++;
                // buckets[pos].stability[i]++;
                return;
            }

            if(buckets[pos].count[i] == 0){
                buckets[pos].ID[i] = item;
                buckets[pos].count[i] = 1;
                // buckets[pos].stability[i] = 1;
                return;
            }

            if(buckets[pos].count[i] < minVal){
                minVal = buckets[pos].count[i];
                minIndex = i;
            }
        }

            // int k = rand() % (int)((buckets[pos].count[minIndex] * buckets[pos].stability[minIndex]) + 1.0) + 1.0;
            // if (k > buckets[pos].count[minIndex] * buckets[pos].stability[minIndex]) {
            //     if (--buckets[pos].count[minIndex] == 0) {
            //         buckets[pos].ID[minIndex] = item;
            //         buckets[pos].count[minIndex] = 1;
            //         buckets[pos].stability[minIndex] = std::max(buckets[pos].stability[minIndex] - 1, 0);
            //     }
            // }

        buckets[pos].count[minIndex]++;
        if (randomGenerator() % buckets[pos].count[minIndex] == 0) {
            buckets[pos].ID[minIndex] = item;
            buckets[pos].count[minIndex] = 1;
        }    
	}

    COUNT_TYPE Query(const DATA_TYPE& item){
        return buckets[hash(item) % LENGTH].Query(item);
    }

    HashMap AllQuery(){
        HashMap ret;
        for(uint32_t i = 0; i < LENGTH; ++i){
            for(uint32_t j = 0; j < COUNTER_PER_BUCKET; ++j) {
                if (buckets[i].ID[j][0] != '\0') {
                    ret[buckets[i].ID[j]] = buckets[i].count[j] + this->stage1_bias;
                }
            }
        }
        return ret;
    }

private:
    uint32_t LENGTH;
    Bucket* buckets;
};

#endif