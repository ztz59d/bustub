#include "primer/trie.h"
#include <string_view>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // throw NotImplementedException("Trie::Get is not implemented.");

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.
  if (key.empty() && root_ != nullptr && root_->is_value_node_) {
    const TrieNodeWithValue<T> *res = dynamic_cast<const TrieNodeWithValue<T> *>(root_.get());
    if (res != nullptr) {
      return res->value_.get();
    }
  }
  std::shared_ptr<const TrieNode> cur = root_;
  // LOG_INFO("the key length is: %lu\n", key.size());

  size_t i = 0;
  for (; i < key.length(); i++) {
    // char c = key[i];
    if (cur->children_.find(key[i]) != cur->children_.end()) {
      // found the node in the path
      cur = cur->children_.at(key[i]);
    } else {
      return nullptr;
    }
  }
  if (cur->is_value_node_) {
    const TrieNodeWithValue<T> *res = dynamic_cast<const TrieNodeWithValue<T> *>(cur.get());
    // return static_cast<const T *>(res->value_.get());
    if (res != nullptr) {
      return res->value_.get();
    }
  }
  return nullptr;
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
  std::shared_ptr<TrieNode> new_root;

  if (root_ == nullptr) {
    new_root = std::make_shared<TrieNode>();
  } else {
    new_root = root_->Clone();
  }
  // LOG_INFO("the key length is: %lu\n", key.size());
  if (key.length() == 0) {
    new_root = std::make_shared<TrieNodeWithValue<T>>(new_root->children_, std::make_shared<T>(std::move(value)));
    return Trie(new_root);
  }
  // std::shared_ptr<const TrieNode> cur_k = root_;
  std::shared_ptr<TrieNode> cur_v = new_root;
  size_t i = 0;
  std::shared_ptr<TrieNode> pre;

  for (; i < key.length(); i++) {
    if (cur_v->children_.find(key[i]) == cur_v->children_.end()) {
      if (i < key.length() - 1) {
        pre = std::make_shared<TrieNode>();
        cur_v->children_[key[i]] = pre;
        cur_v = pre;
        // break;
      } else {
        pre = std::make_shared<TrieNodeWithValue<T>>(std::make_shared<T>(std::move(value)));
        cur_v->children_[key[i]] = pre;
        cur_v = pre;
      }

    } else {
      if (i < key.length() - 1) {
        pre = cur_v;
        cur_v.reset(cur_v->children_[key[i]]->Clone().release());
        pre->children_[key[i]] = cur_v;
      } else {
        pre = cur_v;
        cur_v = std::make_shared<TrieNodeWithValue<T>>(cur_v->children_[key[i]]->children_,
                                                       std::make_shared<T>(std::move(value)));
        pre->children_[key[i]] = cur_v;
        // return new_root;
      }

      // cur_k = cur_k->children_[key[i]];
    }
  }
  // if (i == key.length()) {
  //   return *this;
  // }
  return Trie(new_root);
}

auto Trie::Remove(std::string_view key) const -> Trie {
  throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked
// up by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
