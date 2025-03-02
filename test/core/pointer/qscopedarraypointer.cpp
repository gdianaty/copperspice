/***********************************************************************
*
* Copyright (c) 2012-2023 Barbara Geller
* Copyright (c) 2012-2023 Ansel Sermersheim
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://www.gnu.org/licenses/
*
***********************************************************************/

#include <qobject.h>
#include <qscopedarraypointer.h>

#include <cs_catch2.h>

TEST_CASE("QScopedArrayPointer traits", "[qscopedarraypointer]")
{
   // without brackets
   REQUIRE(std::is_copy_constructible_v<QScopedArrayPointer<int>> == false);
   REQUIRE(std::is_move_constructible_v<QScopedArrayPointer<int>> == true);

   REQUIRE(std::is_copy_assignable_v<QScopedArrayPointer<int>> == false);
   REQUIRE(std::is_move_assignable_v<QScopedArrayPointer<int>> == true);

   REQUIRE(std::has_virtual_destructor_v<QScopedArrayPointer<int>> == false);

   // with brackets
   REQUIRE(std::is_copy_constructible_v<QScopedArrayPointer<int[]>> == false);
   REQUIRE(std::is_move_constructible_v<QScopedArrayPointer<int[]>> == true);

   REQUIRE(std::is_copy_assignable_v<QScopedArrayPointer<int[]>> == false);
   REQUIRE(std::is_move_assignable_v<QScopedArrayPointer<int[]>> == true);

   REQUIRE(std::has_virtual_destructor_v<QScopedArrayPointer<int[]>> == false);
}

TEST_CASE("QScopedArrayPointer convert", "[qscopedarraypointer]")
{
   QScopedArrayPointer<int[]> ptr1 = QMakeScoped<int[]>(1);
   ptr1[0] = 42;

   std::unique_ptr<int[]> ptr2 = std::move(ptr1);

   REQUIRE(ptr1 == nullptr);
   REQUIRE(ptr2 != nullptr);

   REQUIRE(ptr2[0] == 42);


   // no brackets
   QScopedArrayPointer<double> ptr3 = QMakeScoped<double[]>(1);
   ptr3[0] = 42.5;

   std::unique_ptr<double[]> ptr4 = std::move(ptr3);

   REQUIRE(ptr3 == nullptr);
   REQUIRE(ptr4 != nullptr);

   REQUIRE(ptr4[0] == 42.5);
}

TEST_CASE("QScopedArrayPointer empty", "[qscopedarraypointer]")
{
   QScopedArrayPointer<int[]> ptr;

   REQUIRE(ptr == nullptr);
   REQUIRE(nullptr == ptr);
   REQUIRE(ptr == ptr);

   REQUIRE(! (ptr != nullptr));
   REQUIRE(! (nullptr != ptr));
   REQUIRE(! (ptr != ptr)) ;

   REQUIRE(ptr.isNull() == true);
}

TEST_CASE("QScopedArrayPointer move_assign", "[qscopedarraypointer]")
{
   QScopedArrayPointer<int[]> ptr1;
   int *rawPointer = nullptr;

   {
      QScopedArrayPointer<int[]> ptr2(new int[1]);
      rawPointer = ptr2.data();
      ptr1 = std::move(ptr2);

      REQUIRE(ptr2.isNull());
   }

   REQUIRE(rawPointer == ptr1.get());
}

TEST_CASE("QScopedArrayPointer move_construct", "[qscopedarraypointer]")
{
   QScopedArrayPointer<int[]> ptr1 = QMakeScoped<int[]>(1);
   QScopedArrayPointer<int[]> ptr2(std::move(ptr1));

   REQUIRE(ptr1.isNull() == true);
   REQUIRE(ptr2.isNull() == false);

   QScopedArrayPointer<int[]> ptr3(std::move(ptr2));

   REQUIRE(ptr2.isNull() == true);
   REQUIRE(ptr3.isNull() == false);
}

TEST_CASE("QScopedArrayPointer release", "[qscopedarraypointer]")
{
   QScopedArrayPointer<int[]> ptr = QMakeScoped<int[]>(1);
   int *p1 = ptr.get();
   int *p2 = ptr.release();

   REQUIRE(p1 == p2);
   REQUIRE(ptr == nullptr);

   delete[] p2;

   REQUIRE(ptr.release() == nullptr);
}

TEST_CASE("QScopedArrayPointer reset", "[qscopedarraypointer]")
{
   QScopedArrayPointer<int[]> ptr = QMakeScoped<int[]>(1);
   ptr.reset();

   REQUIRE(ptr == nullptr);
   REQUIRE(ptr.isNull() == true);
}

TEST_CASE("QScopedArrayPointer swap", "[qscopedarraypointer]")
{
   QScopedArrayPointer<int[]> ptr1 = QMakeScoped<int[]>(1);
   QScopedArrayPointer<int[]> ptr2 = QMakeScoped<int[]>(1);

   ptr1[0] = 8;
   ptr2[0] = 17;

   REQUIRE(*ptr1 == 8);
   REQUIRE(*ptr2 == 17);

   ptr1.swap(ptr2);

   REQUIRE(*ptr1 == 17);
   REQUIRE(*ptr2 == 8);

   ptr1.reset();
   ptr1.swap(ptr2);

   REQUIRE(*ptr1 == 8);
   REQUIRE(ptr2 == nullptr);

   ptr1.swap(ptr1);

   REQUIRE(*ptr1 == 8);
   REQUIRE(ptr2 == nullptr);

   ptr2.swap(ptr2);

   REQUIRE(*ptr1 == 8);
   REQUIRE(ptr2 == nullptr);
}
