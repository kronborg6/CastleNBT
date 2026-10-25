#pragma once
namespace castle::nbt {

  enum class TagContext {
    Named,
    ListElement,
    NonCompoundRoot,
    NamelessRoot,
    NamelessRootAndNonCompoundRoot
  };
} // namespace castle::nbt
