/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 *  Copyright (c) 2017 by Contributors
 * \file graph_hash.h
 * \brief The graph hashing function.
 */
#ifndef NNVM_COMPILER_GRAPH_HASH_H_
#define NNVM_COMPILER_GRAPH_HASH_H_

#include <dmlc/common.h>
#include <nnvm/graph.h>
#include <tvm/operation.h>
#include <string>
#include <utility>

namespace nnvm {
namespace compiler {

class GraphKey;

/*! \brief Key to a graph compiler cache */
struct GraphKeyNode : public tvm::Node {
  /*! \brief The graph structure */
  Graph graph;
  /* \brief The inputs to the function */
  tvm::Array<Tensor> inputs;
  /*! \brief The target */
  std::string target;
  // Cached internal hash key, invisible to the user.
  // The graph hash key is ensured always not to be 0
  mutable size_t cache_hash_key_{0};

  void VisitAttrs(tvm::AttrVisitor* v) final {
    v->Visit("inputs", &inputs);
    v->Visit("target", &target);
  }

  static GraphKey make(Graph graph,
                       tvm::Array<Tensor> inputs,
                       std::string target);
  static constexpr const char* _type_key = "GraphKey";
  TVM_DECLARE_NODE_TYPE_INFO(GraphKeyNode, tvm::Node);
};

TVM_DEFINE_NODE_REF(GraphKey, GraphKeyNode);

/*! \brief Hashing function for graph key */
struct GraphKeyHash {
  size_t operator()(const GraphKey& gkey) const {
    return Hash(gkey);
  }
  static size_t Hash(const GraphKey& gkey);
};

/*! \brief function for graph key */
struct GraphKeyEqual {
  bool operator()(const GraphKey& a,
                  const GraphKey& b) const {
    return Equal(a, b);
  }
  static bool Equal(const GraphKey& a, const GraphKey& b);
};

/*!
 * \brief Create a hash code for a given graph.
 * \return The hash code of the graph.
 */
size_t GraphHash(const Graph& graph);

/*!
 * \brief Compare two graphs
 *  return empty string if they are equal
 *  otherwise return error message
 * \param a The first graph.
 * \param b The second graph.
 * \return empty string if they are equal, otherwise return error message.
 */
std::string GraphDeepCompare(const Graph& a,
                             const Graph& b,
                             bool compare_variable_attr);
}  // namespace compiler
}  // namespace nnvm

#endif  // NNVM_COMPILER_GRAPH_HASH_H_
