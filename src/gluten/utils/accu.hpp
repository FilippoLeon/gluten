#pragma once

namespace gluten::accu {

template <typename Rep1, typename Period1, typename Rep2, typename Period2>
typename std::common_type<
    std::chrono::duration<Rep1, Period1>,
    std::chrono::duration<Rep2, Period2>
>::type operator*(
    std::chrono::duration<Rep1, Period1> a,
    std::chrono::duration<Rep2, Period2> b) {
    using CD = typename std::common_type<
        std::chrono::duration<Rep1, Period1>, 
        std::chrono::duration<Rep2, Period2>
    >::type;
    return CD(CD(a).count() * CD(b).count());
}

template <typename T = float>
class Mean {
public:
    Mean() : mean(), m2() { }

    void Add(const T & value) {
        count++;
        T delta = value - mean;
        mean += delta / count;
        T delta_2 = value - mean;
        m2 += delta * delta_2;
    }

    std::tuple<T, T> Get() const {
        if (count <= 1) {
            return { mean, T() };
        } else {
            return { mean, m2 / (count - 1) };
        }
    }

    void Clear() {
        mean = T();
        m2 = T();
        count = 0;
    }

    int Count() const {
        return count;
    }
private:
    unsigned int count = 0;
    T mean, m2;
};

}