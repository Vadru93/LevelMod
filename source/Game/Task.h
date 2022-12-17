#pragma once
#include "RenderWare\RenderWare.h"

#define nTemplateBaseClass(_t,_c)				\
	template< class _t >						\
	class _c : public Spt::Class								\

#define nTemplateBaseClass2(_t1,_t2,_c)			\
	template< class _t1, class _t2 >			\
	class _c : public Spt::Class								\

#define nTemplateBaseClass3(_t1,_t2,_t3,_c)		\
	template< class _t1, class _t2, class _t3 >	\
	class _c : public Spt::Class								\

#define nTemplateSubClass(_t,_c,_b)				\
	template< class _t >						\
	class _c : public _b							\

#define nTemplateSubClass2(_t1,_t2,_c,_b)		\
	template< class _t1, class _t2 >			\
	class _c : public _b							\

#define nTemplateSubClass3(_t1,_t2,_t3,_c,_b)	\
	template< class _t1, class _t2, class _t3 >	\
	class _c : public _b							\


namespace Tsk
{
    class BaseTask : BaseClass
    {
    public:
        virtual	void	vCall(void) const = 0;
        virtual void* GetCode(void) const = 0;

        BaseTask(int prio)
        {
            typedef void* (__thiscall* const pBaseTask)(BaseTask* pThis, int prio);
            pBaseTask(0x00410520)(this, prio);
        }
    };

    class Stack
    {
    public:
        void AddTask(Tsk::BaseTask& task)
        {
            
            
        }
    };


    nTemplateSubClass(_T, Task, BaseTask)
    {


    public:

        typedef void	(Code)(const Task< _T >&);

        Task(Code* const code, _T & data, int pri = 0);

        virtual void	vCall(void) const;
        _T& GetData(void) const;
        virtual void* GetCode(void) const;

    private:
        Code* const		code;			// tasks entry point
        _T& data;			// task defined data
    };

    template < class _T > inline
        Task< _T >::Task(Code* const _code, _T& _data, int pri)
        : BaseTask(pri), code(_code), data(_data)
    {

    }

    /******************************************************************/
    /*                                                                */
    /*                                                                */
    /******************************************************************/

    template < class _T > inline
        void		Task< _T >::vCall(void) const
    {

        code(*this);
    }

    /******************************************************************/
    /*                                                                */
    /*                                                                */
    /******************************************************************/

    template < class _T > inline
        _T& Task< _T >::GetData(void) const
    {

        return data;
    }

    /******************************************************************/
    /*                                                                */
    /*                                                                */
    /******************************************************************/



    template < class _T > inline
        void* Task< _T >::GetCode(void) const
    {


        return (void*)code;
    }
};