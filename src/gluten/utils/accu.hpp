#pragma once

//! \namespace accu
//! \brief Contains definitions for accumulators.
//!
//! Contains classes that allow to accumulate and compute statistics on samples.
//! Table of contents:
//! - Mean/Variance one-pass online constant storage stable accumulator.
namespace gluten::accu {

//! \brief Overload operator to multiply two times.
//!
//! While product of times in not entirely sensical, the product can be
//! used to compute variance and higher moments of time, provided care is taken
//! when considering units of measure.
//!
//! \tparam Rep1    Representation of first time.
//! \tparam Period1 Period of first time.
//! \tparam Rep2    Representation of second time.
//! \tparam Period2 Period of second time.
//! \param  a       The first time.
//! \param  b       The second time.
//! \return Product of two times in unit \f$Rep1^2\f$.
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

//! \brief A very simple mean and variance one-pass accumulator.
//!
//! The class computes mean and variance in a stable, and one-pass manner.
//!
//! \tparam T  The underlying type. Must provide basic arithmetic operators.
template <typename T = float>
class Mean {
public:
    //! \brief Initialize mean and variance with no sample.
    //!
    //! Data is value default-initialized.
    Mean() : mean(), m2() { }

    //! \brief Add a new sample to the collection.
    //!
    //! The value is not stored, only mean and variance will be updated.
    //!
    //! \param value  The value to add to the collection.
    void Add(const T & value) {
        count++;
        T delta = value - mean;
        mean += delta / count;
        T delta_2 = value - mean;
        m2 += delta * delta_2;
    }

    //! \brief Access to mean and variance.
    //!
    //! By convention we say that the sample variance with a single sample is T().
    //!
    //! \return A tuple with sample mean and sample variance of the collection.
    std::tuple<T, T> Get() const {
        if (count <= 1) {
            return { mean, T() };
        } else {
            return { mean, m2 / (count - 1) };
        }
    }

    //! \brief Reset structure to initial state.
    //!
    //! After this call, the class can be used as if it where default initialized.
    void Clear() {
        mean = T();
        m2 = T();
        count = 0;
    }

    //! \brief Access the number of samples added to the collection.
    //!
    //! Returns the count, i.e. number of calls to Add(,) so far.
    //!
    //! \return Number of calls to Add(,)
    int Count() const {
        return count;
    }
private:
    //! Number of calls to Add(,)
    unsigned int count = 0;
    //! Storage values for mean and scaled variance.
    T mean, m2;
};

}