// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_TURANAPIFORMATS_TURANAPIFORMATS_H_
#define FLATBUFFERS_GENERATED_TURANAPIFORMATS_TURANAPIFORMATS_H_

#include "flatbuffers/flatbuffers.h"

namespace TuranAPIFormats {

struct Profiled_Scope;

struct Profiling_Session;

struct Resource;

enum Resource_Type {
  Resource_Type_NONE = 0,
  Resource_Type_Profiling = 1,
  Resource_Type_MIN = Resource_Type_NONE,
  Resource_Type_MAX = Resource_Type_Profiling
};

inline const Resource_Type (&EnumValuesResource_Type())[2] {
  static const Resource_Type values[] = {
    Resource_Type_NONE,
    Resource_Type_Profiling
  };
  return values;
}

inline const char * const *EnumNamesResource_Type() {
  static const char * const names[3] = {
    "NONE",
    "Profiling",
    nullptr
  };
  return names;
}

inline const char *EnumNameResource_Type(Resource_Type e) {
  if (e < Resource_Type_NONE || e > Resource_Type_Profiling) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesResource_Type()[index];
}

template<typename T> struct Resource_TypeTraits {
  static const Resource_Type enum_value = Resource_Type_NONE;
};

template<> struct Resource_TypeTraits<TuranAPIFormats::Profiling_Session> {
  static const Resource_Type enum_value = Resource_Type_Profiling;
};

bool VerifyResource_Type(flatbuffers::Verifier &verifier, const void *obj, Resource_Type type);
bool VerifyResource_TypeVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types);

struct Profiled_Scope FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_START_POINT = 4,
    VT_END_POINT = 6,
    VT_THREAD_ID = 8,
    VT_NAME = 10
  };
  float START_POINT() const {
    return GetField<float>(VT_START_POINT, 0.0f);
  }
  float END_POINT() const {
    return GetField<float>(VT_END_POINT, 0.0f);
  }
  uint8_t THREAD_ID() const {
    return GetField<uint8_t>(VT_THREAD_ID, 0);
  }
  const flatbuffers::String *NAME() const {
    return GetPointer<const flatbuffers::String *>(VT_NAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<float>(verifier, VT_START_POINT) &&
           VerifyField<float>(verifier, VT_END_POINT) &&
           VerifyField<uint8_t>(verifier, VT_THREAD_ID) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(NAME()) &&
           verifier.EndTable();
  }
};

struct Profiled_ScopeBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_START_POINT(float START_POINT) {
    fbb_.AddElement<float>(Profiled_Scope::VT_START_POINT, START_POINT, 0.0f);
  }
  void add_END_POINT(float END_POINT) {
    fbb_.AddElement<float>(Profiled_Scope::VT_END_POINT, END_POINT, 0.0f);
  }
  void add_THREAD_ID(uint8_t THREAD_ID) {
    fbb_.AddElement<uint8_t>(Profiled_Scope::VT_THREAD_ID, THREAD_ID, 0);
  }
  void add_NAME(flatbuffers::Offset<flatbuffers::String> NAME) {
    fbb_.AddOffset(Profiled_Scope::VT_NAME, NAME);
  }
  explicit Profiled_ScopeBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Profiled_ScopeBuilder &operator=(const Profiled_ScopeBuilder &);
  flatbuffers::Offset<Profiled_Scope> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Profiled_Scope>(end);
    return o;
  }
};

inline flatbuffers::Offset<Profiled_Scope> CreateProfiled_Scope(
    flatbuffers::FlatBufferBuilder &_fbb,
    float START_POINT = 0.0f,
    float END_POINT = 0.0f,
    uint8_t THREAD_ID = 0,
    flatbuffers::Offset<flatbuffers::String> NAME = 0) {
  Profiled_ScopeBuilder builder_(_fbb);
  builder_.add_NAME(NAME);
  builder_.add_END_POINT(END_POINT);
  builder_.add_START_POINT(START_POINT);
  builder_.add_THREAD_ID(THREAD_ID);
  return builder_.Finish();
}

inline flatbuffers::Offset<Profiled_Scope> CreateProfiled_ScopeDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    float START_POINT = 0.0f,
    float END_POINT = 0.0f,
    uint8_t THREAD_ID = 0,
    const char *NAME = nullptr) {
  auto NAME__ = NAME ? _fbb.CreateString(NAME) : 0;
  return TuranAPIFormats::CreateProfiled_Scope(
      _fbb,
      START_POINT,
      END_POINT,
      THREAD_ID,
      NAME__);
}

struct Profiling_Session FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_PROFILED_SCOPES = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<TuranAPIFormats::Profiled_Scope>> *Profiled_Scopes() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<TuranAPIFormats::Profiled_Scope>> *>(VT_PROFILED_SCOPES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_PROFILED_SCOPES) &&
           verifier.VerifyVector(Profiled_Scopes()) &&
           verifier.VerifyVectorOfTables(Profiled_Scopes()) &&
           verifier.EndTable();
  }
};

struct Profiling_SessionBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_Profiled_Scopes(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TuranAPIFormats::Profiled_Scope>>> Profiled_Scopes) {
    fbb_.AddOffset(Profiling_Session::VT_PROFILED_SCOPES, Profiled_Scopes);
  }
  explicit Profiling_SessionBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  Profiling_SessionBuilder &operator=(const Profiling_SessionBuilder &);
  flatbuffers::Offset<Profiling_Session> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Profiling_Session>(end);
    return o;
  }
};

inline flatbuffers::Offset<Profiling_Session> CreateProfiling_Session(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TuranAPIFormats::Profiled_Scope>>> Profiled_Scopes = 0) {
  Profiling_SessionBuilder builder_(_fbb);
  builder_.add_Profiled_Scopes(Profiled_Scopes);
  return builder_.Finish();
}

inline flatbuffers::Offset<Profiling_Session> CreateProfiling_SessionDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<flatbuffers::Offset<TuranAPIFormats::Profiled_Scope>> *Profiled_Scopes = nullptr) {
  auto Profiled_Scopes__ = Profiled_Scopes ? _fbb.CreateVector<flatbuffers::Offset<TuranAPIFormats::Profiled_Scope>>(*Profiled_Scopes) : 0;
  return TuranAPIFormats::CreateProfiling_Session(
      _fbb,
      Profiled_Scopes__);
}

struct Resource FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_TYPE_TYPE = 4,
    VT_TYPE = 6
  };
  TuranAPIFormats::Resource_Type TYPE_type() const {
    return static_cast<TuranAPIFormats::Resource_Type>(GetField<uint8_t>(VT_TYPE_TYPE, 0));
  }
  const void *TYPE() const {
    return GetPointer<const void *>(VT_TYPE);
  }
  template<typename T> const T *TYPE_as() const;
  const TuranAPIFormats::Profiling_Session *TYPE_as_Profiling() const {
    return TYPE_type() == TuranAPIFormats::Resource_Type_Profiling ? static_cast<const TuranAPIFormats::Profiling_Session *>(TYPE()) : nullptr;
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_TYPE_TYPE) &&
           VerifyOffset(verifier, VT_TYPE) &&
           VerifyResource_Type(verifier, TYPE(), TYPE_type()) &&
           verifier.EndTable();
  }
};

template<> inline const TuranAPIFormats::Profiling_Session *Resource::TYPE_as<TuranAPIFormats::Profiling_Session>() const {
  return TYPE_as_Profiling();
}

struct ResourceBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_TYPE_type(TuranAPIFormats::Resource_Type TYPE_type) {
    fbb_.AddElement<uint8_t>(Resource::VT_TYPE_TYPE, static_cast<uint8_t>(TYPE_type), 0);
  }
  void add_TYPE(flatbuffers::Offset<void> TYPE) {
    fbb_.AddOffset(Resource::VT_TYPE, TYPE);
  }
  explicit ResourceBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ResourceBuilder &operator=(const ResourceBuilder &);
  flatbuffers::Offset<Resource> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Resource>(end);
    return o;
  }
};

inline flatbuffers::Offset<Resource> CreateResource(
    flatbuffers::FlatBufferBuilder &_fbb,
    TuranAPIFormats::Resource_Type TYPE_type = TuranAPIFormats::Resource_Type_NONE,
    flatbuffers::Offset<void> TYPE = 0) {
  ResourceBuilder builder_(_fbb);
  builder_.add_TYPE(TYPE);
  builder_.add_TYPE_type(TYPE_type);
  return builder_.Finish();
}

inline bool VerifyResource_Type(flatbuffers::Verifier &verifier, const void *obj, Resource_Type type) {
  switch (type) {
    case Resource_Type_NONE: {
      return true;
    }
    case Resource_Type_Profiling: {
      auto ptr = reinterpret_cast<const TuranAPIFormats::Profiling_Session *>(obj);
      return verifier.VerifyTable(ptr);
    }
    default: return false;
  }
}

inline bool VerifyResource_TypeVector(flatbuffers::Verifier &verifier, const flatbuffers::Vector<flatbuffers::Offset<void>> *values, const flatbuffers::Vector<uint8_t> *types) {
  if (!values || !types) return !values && !types;
  if (values->size() != types->size()) return false;
  for (flatbuffers::uoffset_t i = 0; i < values->size(); ++i) {
    if (!VerifyResource_Type(
        verifier,  values->Get(i), types->GetEnum<Resource_Type>(i))) {
      return false;
    }
  }
  return true;
}

inline const TuranAPIFormats::Resource *GetResource(const void *buf) {
  return flatbuffers::GetRoot<TuranAPIFormats::Resource>(buf);
}

inline const TuranAPIFormats::Resource *GetSizePrefixedResource(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<TuranAPIFormats::Resource>(buf);
}

inline bool VerifyResourceBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<TuranAPIFormats::Resource>(nullptr);
}

inline bool VerifySizePrefixedResourceBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<TuranAPIFormats::Resource>(nullptr);
}

inline void FinishResourceBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<TuranAPIFormats::Resource> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedResourceBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<TuranAPIFormats::Resource> root) {
  fbb.FinishSizePrefixed(root);
}

}  // namespace TuranAPIFormats

#endif  // FLATBUFFERS_GENERATED_TURANAPIFORMATS_TURANAPIFORMATS_H_
