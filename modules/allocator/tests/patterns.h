
#pragma once

#include "strings.h"

#include <iostream>

#include "collector.h"

tp::alni hash(const tp::string& val);

#include "map.h"

enum class leav_pattern_type {
  LINEAR,
  RANDOM,
  SINE,
  CONST,
};

struct child_pattern {
  child_pattern() {}

  child_pattern(tp::string _name) { name = _name; }

  float uppernlim = 1.f;
  float lowerlim = 0.f;
  float point = 1.f;
  tp::string name;
};

struct pattern {
  leav_pattern_type type = leav_pattern_type::CONST;
  tp::string pattern_name;

  tp::Array<child_pattern> regions;
  bool build_in = true;

  tp::alnf get_y(tp::HashMap<pattern*, tp::string>* patterns, tp::alnf x) {
    assert(x <= 1.0001f && x >= -0.00001f);

    if (!regions.length()) {
      return pure_get_y(x);
    }

    float offset = 0.f;
    for (tp::alni i = 0; i < regions.length(); i++) {
      tp::alni idx = patterns->presents(regions[i].name);
      if (!MAP_VALID_IDX(idx)) {
        return 0.f;
      }
      pattern* child = (*patterns)[idx];
      assert(child);

      float range = regions[i].point * (1.f - offset);
      if (offset + range > x) {
        return regions[i].lowerlim +
               (child->get_y(patterns, (x - offset) / range) *
                (regions[i].uppernlim - regions[i].lowerlim));
      }
      offset += range;
    }
    return 0;
  }

  virtual tp::alnf pure_get_y(tp::alnf x) { return 0; }

  ~pattern() {}
};

// -------------------- build-in patterns ---------------------------- //

struct const_pattern : pattern {
  float val = 1.f;
  const_pattern() {
    type = leav_pattern_type::CONST;
    pattern_name = "const";
    build_in = true;
  }

  tp::alnf pure_get_y(tp::alnf x) override { return val; }
};

struct linear_pattern : pattern {
  bool reversed = false;
  linear_pattern() {
    type = leav_pattern_type::LINEAR;
    pattern_name = "linear";
    build_in = true;
  }

  tp::alnf pure_get_y(tp::alnf x) override { return reversed ? 1.f - x : x; }
};

struct random_pattern : pattern {
  random_pattern() {
    type = leav_pattern_type::RANDOM;
    build_in = true;
  }

  tp::alnf pure_get_y(tp::alnf x) override { return tp::randf(); }
};

// -------------------- build-in patterns end ---------------------------- //

struct pattern_scale {
  int items = 0;
  int size = 0;
  int iterations = 0;
};

class pattern_reader : public test_pattern {
 public:
  bool init(tp::HashMap<pattern*, tp::string>* p_patterns, pattern* p_lpattern,
            pattern* p_opattern, pattern* p_spattern, pattern_scale* p_scale) {
    lpattern = p_lpattern;
    opattern = p_opattern;
    spattern = p_spattern;

    scale = p_scale;

    patterns = p_patterns;
    return verify_rulles();
  }

  bool verify_rulles() {
    if (!lpattern || !opattern || !spattern) {
      return false;
    }

    bool out = true;
    out &= scale->items > 0;
    out &= scale->size > 0;
    out &= scale->iterations > 0;
    return out;
  }

  tp::HashMap<pattern*, tp::string>* patterns;

  pattern* lpattern = NULL;
  pattern* opattern = NULL;
  pattern* spattern = NULL;

  pattern_scale* scale = 0;

  tp::alnf get_x_val(tp::alni iter) {
    tp::alnf out = iter / (tp::alnf)scale->iterations;
    return out > 1 ? 1.f : out;
  }

  tp::alni pick_size(tp::alni iter) override {
    return (tp::alni)(scale->size * spattern->get_y(patterns, get_x_val(iter)));
  }

  tp::alni pick_alloc_count(tp::alni iter) override {
    return (tp::alni)(scale->items * lpattern->get_y(patterns, get_x_val(iter)));
  }

  tp::alni pick_idx(tp::alni iter) override {
    tp::alni out = (tp::alni) (scale->items * opattern->get_y(patterns, get_x_val(iter)));
    CLAMP(out, 0, scale->items - 1);
    return out;
  }

  tp::alni max_size() override { return scale->size; }

  tp::alni max_iterations() override { return scale->iterations; }

  tp::alni data_count() override { return scale->iterations; }
};
