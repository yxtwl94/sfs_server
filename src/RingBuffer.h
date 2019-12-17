//
// Created by yxt on 30.11.19.
//


/*
 *
 * A ring buffer which use 2 iovec struct, but the second iovec point to the begin of the buff so it's same
 *
 * ＋－－－－－－－－－＋－－－－－－－－－＋－－－－－－－－－＋
 * ｜　　　　　　　　　｜　　　　　　　　　｜　　　　　　　　　｜
 * ｜　　　　　　　　　｜　　　　　　　　　｜　　　　　　　　　｜
 * ｜writableBytesL　　  readableBytes      writableBytesR｜
 * ｜　　　　　　　　　｜　　　　　　　　　｜　　　　　　　　　｜
 * ｜　　　　　　　　　｜　　　　　　　　　｜　　　　　　　　　｜
 * ＋－－－－－－－－－＋－－－－＞＞－－－＋－－－－－－－－－＋
 * 0           readIndex       writeIndex         size
 *
 * ==========================================================
 * when readableBytes change writableBytesR/L move like this
 *
 * ＋－－－－－－－－－＋－－－－－－－－－＋－－－－－－－－－＋
 * ｜　　　　　　　　　｜　　　　　　　　　｜　　　　　　　　　｜
 * ｜　　　　　　　　　｜　　　　　　　　　｜　　　　　　　　　｜
 * ｜readableBytes　　  writableBytesR      readableBytes ｜
 * ｜　　　　　　　　　｜　　　　　　　　　｜　　　　　　　　　｜
 * ＋－－－＞＞－－－－＋－－－－－－－－－＋－－－－＞＞－－－＋
 * 0           writeIndex       readIndex         size
 *
 * writableBytesL disappears
 * =========================================================
 *
 * read data is from readIndex to writeIndex
 *
 * */

#ifndef SFS_SERVER_RINGBUFFER_H
#define SFS_SERVER_RINGBUFFER_H

#include <iostream>
#include <algorithm>
#include <vector>

class RingBuffer {

public:
    static const size_t initSize = 1024;
    explicit RingBuffer(size_t initialSize = initSize);

    size_t readableBytes() const {
        if(writeIndex_==readIndex_ && buf_[readIndex_]=='\0') return 0;
        else if(writeIndex_==readIndex_) return buf_.size();

        if(writeIndex_>readIndex_) return writeIndex_-readIndex_;
        else return buf_.size()-(readIndex_-writeIndex_);
    };

    size_t writableBytesR() const {            //right writable
        //等于情况
        if(writeIndex_==readIndex_ && buf_[readIndex_]=='\0') return buf_.size()-writeIndex_-1;
        else if(writeIndex_==readIndex_) return 0;

        if(writeIndex_>readIndex_)
            return buf_.size()-writeIndex_-1;
        else
            return readIndex_-writeIndex_;
    };
    size_t writableBytesL() const {
        if(writeIndex_==readIndex_ && buf_[readIndex_]=='\0') return readIndex_+1;
        else if(writeIndex_==readIndex_) return 0;

        if(writeIndex_>readIndex_)
            return readIndex_+1;   //lef writable
        else
            return 0;
    };

    ssize_t readFromFd(int fd);
    std::string getBuffer();

private:

    std::vector<char> buf_;
    //boost::circular_buffer<char> buf_;
    size_t readIndex_;  //无符号　因为均大于０
    size_t writeIndex_;

    char* begin()
    { return &*buf_.begin(); }

    const char* begin() const
    { return &*buf_.begin(); }
};


#endif //SFS_SERVER_RINGBUFFER_H
