/*
 * Stacks.cpp
 *
 *  Created on: 6 Nov 2015
 *      Author: hieu
 */

#include "Stacks.h"
#include "../Manager.h"
#include "../../System.h"

using namespace std;

namespace Moses2
{

namespace NSBatch
{

Stacks::Stacks(const Manager &mgr) :
    m_mgr(mgr)
{
  // TODO Auto-generated constructor stub

}

Stacks::~Stacks()
{
  for (size_t i = 0; i < m_stacks.size(); ++i) {
    delete m_stacks[i];
  }
}

void Stacks::Init(const Manager &mgr, size_t numStacks)
{
  m_stacks.resize(numStacks);
  for (size_t i = 0; i < m_stacks.size(); ++i) {
    m_stacks[i] = new Stack(mgr);
  }
}

void Stacks::Debug(std::ostream &out, const System &system) const
{
  for (size_t i = 0; i < GetSize(); ++i) {
    const Stack *stack = m_stacks[i];
    if (stack) {
      out << stack->GetSize() << " ";
    }
    else {
      out << "N ";
    }
  }
}

void Stacks::Add(Hypothesis *hypo, Recycler<HypothesisBase*> &hypoRecycle,
    ArcLists &arcLists)
{
  size_t numWordsCovered = hypo->GetBitmap().GetNumWordsCovered();
  //cerr << "numWordsCovered=" << numWordsCovered << endl;
  Stack &stack = *m_stacks[numWordsCovered];
  StackAdd added = stack.Add(hypo);

  size_t nbestSize = m_mgr.system.options.nbest.nbest_size;
  if (nbestSize) {
    arcLists.AddArc(added.added, hypo, added.other);
  }
  else {
    if (!added.added) {
      hypoRecycle.Recycle(hypo);
    }
    else if (added.other) {
      hypoRecycle.Recycle(added.other);
    }
  }
}

}
}
