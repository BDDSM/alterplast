/*
 * Copyright (C) Sergey P. Derevyago, 2003-2004.
 *
 * Permission to copy, use, modify, sell and distribute this software is granted
 * provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied warranty, and
 * with no claim as to its suitability for any purpose.
 *
 */

/** @file
 * ����������� ������� sh_ptr � sh_array.
 */

#ifndef __SH_PTR_HPP__
 #define __SH_PTR_HPP__

#include "fix_alloc.hpp"

/**
 * ������ ����� �������� ����������� "����� ����������" � ��������� ������ ��
 * ������ ���� T. � ��� ������ ������ �������������� ����������� �����������
 * �������, ����� �� ���� �� ��������� ������ ������.
 * ���� ���������� "�������" �������� ���������� ���������� � ���� ����� sh_ptr,
 * �� ����� ��������������� ������� set(0), � ���������� ���� ���������� sh_ptr
 * ������ �� ���������, �.�. delete 0 �������� ������ ���������.
 */
template <class T>
class sh_ptr {
      /** ����������� ������������� */
      struct Rep {
             /** ��������� �� ����������� ������ */
             T* ptr;
             /** ���������� ������ */
             size_t refs;

             /** ������� ������������� � ������������ ������� */
             Rep(T* ptr_) : ptr(ptr_), refs(1) {}

             /** ������� ����������� ������ */
             ~Rep() { delete ptr; }

             /** ��� ��������� ������ */
             void* operator new(size_t)
             {
              return fixed_alloc<Rep>::alloc();
             }

             /** ��� ��������� ������ */
             void operator delete(void* ptr, size_t)
             {
              fixed_alloc<Rep>::free(ptr);
             }
      };

      /** ��������� �� ����������� ������������� */
      Rep* rep;

      /**
       * ��������� ���������� ������
       */
      void empty() const { 
		  if (0 == --rep->refs) 
			  delete rep;
	  }
      
 public:
      /**
       * ������� ������, ������������� �� ����������� ����������� ���������.
       * ��������� ����� ��������� ������ ���� � ������ ������������� ����������
       * � ������������ sh_ptr, �.�. ����� sh_ptr<T>(new T) �� �������� � ������
       * ��������. ��� �� �����, �� ����� ��������, ��� ����� �����
       * f(sh_ptr<T1>(new T1), sh_ptr<T2>(new T2)) ��� ������ ������� ��������,
       * �.�. ������� ���������� ������������ �� ��������� � ����� ������� T1 �
       * T2 ����� ���� ������� �� ��������������� sh_ptr, ���������� ��
       * ������������ ��������.
       */
      explicit sh_ptr(T* ptr=0)
      {
       try { rep=new Rep(ptr); }
       catch (...) {
             delete ptr;
             throw;
       }
      }

      /**
       * �������� ���������� ������ ������ ���������� ���������� ������ ��
       * ����������� ���������.
       */
      sh_ptr(const sh_ptr& shp)
      {
       rep=shp.rep;
       rep->refs++;
      }

      /**
       * ��������� ���������� ������ �� ����������� ��������� � ������� ���,
       * ���� �� ���� ����� ������ �� ���������.
       */
      ~sh_ptr() { empty(); }

      /**
       * ����������� ����� ��������. ���������� ������ �� ������ �����������
       * ��������� ����������� (��� ����� �������� � ��� ��������), � �� �����
       * -- �������������.
       */
      sh_ptr& operator=(const sh_ptr& shp)
      {
       shp.rep->refs++;
       empty();
       rep=shp.rep;
       
       return *this;
      }

      /**
       * ���������� ���������� ��������.
       */
      void swap(sh_ptr& shp)
      {
       Rep* tmp=rep;
       rep=shp.rep;
       shp.rep=tmp;
      }

      /**
       * ���������� ������ �� ����������� ������ ��� ���������� � sh_ptr
       * ��������� �������������.
       */
      T& operator*() const { return *rep->ptr; }

      /**
       * ���������� ��������� �� ����������� ������ ��� ���������� � sh_ptr
       * ��������� ->.
       */
      T* operator->() const { return rep->ptr; }

      /**
       * ���������� �������� ������������ ���������.
       */
      T* get() const { return rep->ptr; }

      /**
       * ������������� �������� ������������ ���������, �.�. ��� ����� ��������
       * sh_ptr ������ ��������� �� ����� ������.
       */
      void set(T* ptr) { rep->ptr=ptr; }

      /**
       * ���������� ���������� ������.
       */
      size_t refs() const { return rep->refs; }

      /** artbear
       * ��������� ���������
       */

      bool isNull() const { return NULL == rep->ptr; }
      bool isValid() const { return !isNull(); }
      bool operator !() const	{ return isNull(); }
     
	  // ��� ���� ������� ������ ������ ������������ � ����������� STL
	  bool operator==(const sh_ptr& rhs) const
	 	{ return rep == rhs.rep; }
      bool operator!=(const sh_ptr& rhs) const
 		{ return !operator ==(rhs); }

};

/**
 * ������ ����� �������� ����������� "����� ����������" � ��������� ������ ��
 * ������ �������� ���� T. � ��� ������ ������ �������������� �����������
 * ����������� �������, ����� �� ���� �� ��������� ������ ������. ����
 * ���������� "�������" �������� ���������� ���������� � ���� ����� sh_array,
 * �� ����� ��������������� ������� set(0), � ���������� ���� ����������
 * sh_array ������ �� ���������, �.�. delete 0 �������� ������ ���������.
 * ��� �������, ������������� ������� "������ ���������" �� ������ �� �����
 * ������� ���� � ����� ������� ���������� sh_ptr<std::vector>. 
 */
template <class T>
class sh_array {
      /** ����������� ������������� */
      struct Rep {
             /** ��������� �� ����������� ������ �������� */
             T* ptr;
             /** ���������� ������ */
             size_t refs;

             /** ������� ������������� � ������������ ������� */
             Rep(T* ptr_) : ptr(ptr_), refs(1) {}

             /** ������� ����������� ������ �������� */
             ~Rep() { delete [] ptr; }

             /** ��� ��������� ������ */
             void* operator new(size_t)
             {
              return fixed_alloc<Rep>::alloc();
             }

             /** ��� ��������� ������ */
             void operator delete(void* ptr, size_t)
             {
              fixed_alloc<Rep>::free(ptr);
             }
      };

      /** ��������� �� ����������� ������������� */
      Rep* rep;
      
 public:
      /**
       * ������� ������, ������������� �� ����������� ����������� ��������� ��
       * ������ ��������. ��������� ����� ��������� ������ ���� � ������
       * ������������� ���������� � ������������ sh_array, �.�. �����
       * sh_array<T>(new T[10]) �� �������� � ������ ��������.
       */
      explicit sh_array(T* ptr=0)
      {
       try { rep=new Rep(ptr); }
       catch (...) {
             delete [] ptr;
             throw;
       }
      }

      /**
       * �������� ���������� ������ ������ ���������� ���������� ������ ��
       * ����������� ���������.
       */
      sh_array(const sh_array& sha)
      {
       rep=sha.rep;
       rep->refs++;
      }

      /**
       * ��������� ���������� ������ �� ����������� ��������� � ������� ���,
       * ���� �� ���� ����� ������ �� ���������.
       */
      ~sh_array() { if (--rep->refs==0) delete rep; }

      /**
       * ����������� ����� ��������. ���������� ������ �� ������ �����������
       * ��������� ����������� (��� ����� �������� � ��� ��������), � �� �����
       * -- �������������.
       */
      sh_array& operator=(const sh_array& sha)
      {
       sha.rep->refs++;
       if (--rep->refs==0) delete rep;
       rep=sha.rep;
       
       return *this;
      }

      /**
       * ���������� ���������� ��������.
       */
      void swap(sh_array& sha)
      {
       Rep* tmp=rep;
       rep=sha.rep;
       sha.rep=tmp;
      }

      /**
       * ���������� ������ �� i-��� ������� ������������ �������.
       */
      T& operator[](size_t i) const { return rep->ptr[i]; }

      /**
       * ���������� �������� ������������ ���������.
       */
      T* get() const { return rep->ptr; }

      /**
       * ������������� �������� ������������ ���������, �.�. ��� ����� ��������
       * sh_array ������ ��������� �� ����� ������ ��������.
       */
      void set(T* ptr) { rep->ptr=ptr; }

      /**
       * ���������� ���������� ������.
       */
      size_t refs() const { return rep->refs; }
};

#endif
