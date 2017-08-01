#pragma once
#ifndef INCLUDES_H
#define INCLUDES_H

#ifdef _DEBUG
constexpr bool debug = true;
#else
constexpr bool debug = false;
#endif

#include "Param.h"
#include "Error.h"
#include "Point.h"
#include "Cell.h"
#include "Matrix.h"
#include "Dinmat.h"
#include "Elipsoid.h"
#endif
