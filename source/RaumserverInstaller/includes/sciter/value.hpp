/** \mainpage Terra Informatica Sciter engine.
 *
 * \section legal_sec In legalese
 *
 * The code and information provided "as-is" without
 * warranty of any kind, either expressed or implied.
 *
 * <a href="http://terrainformatica.com/Sciter">Sciter Home</a>
 *
 * (C) 2003-2014, Terra Informatica Software, Inc. and Andrew Fedoniouk
 *
 **/

/**\file
 * \brief value.hpp VALUE C++ wrapper
 **/

#ifndef __value_hpp__
#define __value_hpp__

  #include "value.h"

  #include <string>
  #include <functional>
  #include "aux-slice.h"
  #include "aux-cvt.h"

  #pragma warning( push )
  #pragma warning(disable:4786) //identifier was truncated...

  namespace sciter
  {

    template<typename TC>
      inline size_t str_length(const TC* src ) { 
        if(!src) return 0;
        size_t cnt = 0; while( *src++ ) ++cnt;
        return cnt;
      }
    
    // wide (utf16) string
    typedef std::basic_string<WCHAR> string;
    // ascii or utf8 string
    typedef std::basic_string<char>  astring;

    typedef std::runtime_error script_error;

    // value by key bidirectional proxy/accessor 
    class value_key_a;
    // value by index bidirectional proxy/accessor 
    class value_idx_a;
    class value;

#ifdef CPP11
    // native function that can be stored inside the value:
    typedef std::function<value(unsigned int argc, const value* argv)> native_function_t;
#endif

    class value: public VALUE
    {
      value(void*) {} // no such thing, sorry
      //void* get(const void* defv) const { return 0; } // and this one too is disabled
      //void* get(const void* defv) { return 0; } // and this one too is disabled
    public:
      value()                 { ValueInit(this); }
     ~value()                 { ValueClear(this); }
      value(const value& src) { ValueInit(this); ValueCopy(this,&src); }
      value(const VALUE& src) { ValueInit(this); ValueCopy(this,&src); }

      value& operator = (const value& src) { ValueCopy(this,&src); return *this; }
      value& operator = (const VALUE& src) { ValueCopy(this,&src); return *this; }

      value( bool v )           { ValueInit(this); ValueIntDataSet(this, v?1:0, T_BOOL, 0); }
      value( int  v )           { ValueInit(this); ValueIntDataSet(this, v, T_INT, 0); }
      value( double v )         { ValueInit(this); ValueFloatDataSet(this, v, T_FLOAT, 0); }

      value( const WCHAR* s, unsigned int slen = 0 ) { ValueInit(this); ValueStringDataSet(this, LPCWSTR(s), (slen || !s)? slen : (unsigned int)str_length(s), 0); }
      value( const string& s ) { ValueInit(this); ValueStringDataSet(this, LPCWSTR(s.c_str()), UINT(s.length()), 0); }
      value( const astring& s ) { aux::a2w as(s.c_str()); ValueInit(this); ValueStringDataSet(this, LPCWSTR(as.c_str()), UINT(as.length()), UT_STRING_SYMBOL); }
      //value( const std::ustring& s ) { ValueInit(this); ValueStringDataSet(this, LPCWSTR(s.c_str()), UINT(s.length()), 0); }
      //value( const std::string& s ) { aux::a2w as(s.c_str()); ValueInit(this); ValueStringDataSet(this, LPCWSTR(as.c_str()), UINT(as.length()), UT_STRING_SYMBOL); }
      value( aux::wchars ws )   { ValueInit(this); ValueStringDataSet(this, LPCWSTR(ws.start), ws.length, 0); }

      value( aux::bytes bs )    { ValueInit(this); ValueBinaryDataSet(this, bs.start, bs.length, T_BYTES, 0); }

      value( const value* arr, unsigned n )  { ValueInit(this); for( unsigned i = 0; i < n; ++i ) set_item(int(i),arr[i]); }
#ifdef CPP11
      value( const native_function_t& nfr );
#endif
          
      static value currency( INT64 v )  { value t; ValueInt64DataSet(&t, v, T_CURRENCY, 0); return t;}
      static value date( INT64 v, bool is_utc = true /* true if ft is UTC*/ )      { value t; ValueInt64DataSet(&t, v, T_DATE, is_utc);  return t;}
#ifdef WIN32
      static value date( FILETIME ft, bool is_utc = true /* true if ft is UTC*/ )  { value t; ValueInt64DataSet(&t, *((INT64*)&ft), T_DATE, is_utc); return t;} 
#endif
      static value symbol( aux::wchars wc ) { value t; ValueInit(&t); ValueStringDataSet(&t, LPCWSTR(wc.start), wc.length , 0xFFFF); return t; }

      // string-symbol
      value( const char* s ) 
      { 
        aux::a2w as(s);
        ValueInit(this); ValueStringDataSet(this, LPCWSTR(as.c_str()), UINT(as.length()), UT_STRING_SYMBOL);
      }
            
      static value make_string( const char* s ) 
      { 
        aux::a2w as(s);
        return value(as.chars());
      }
      static value make_string( const WCHAR* s )
      { 
        return value( aux::chars_of(s) );
      }

      /** Creates an array of values packaged into the value 
          Creates an empty array if called with length == 0 */
      static value make_array( unsigned int length = 0, const value* elements = nullptr )
      {
        value v;
        ValueIntDataSet(&v, INT(length), T_ARRAY, 0);
        if( elements ) 
          for( unsigned int i = 0; i < length; ++i ) 
           v.set_item(INT(i),elements[i]);
        return v;
      }

      /** Creates an empty json key/value map (object in JS terms) 
          The map can be populated by map.set_item(key,val); */
      static value make_map(  )
      {
        value v;
        ValueIntDataSet(&v, INT(0), T_MAP, 0);
        return v;
      }
      
      static value secure_string(const WCHAR* s, size_t slen)
      {
        value v;
        ValueStringDataSet(&v, LPCWSTR(s), UINT(slen), UT_STRING_SECURE);
        return v;
      }

      static value make_error(const WCHAR* s) // returns string representing error. 
                                              // if such value is used as a return value from native function
                                              // the script runtime will throw an error in script rather than returning that value.
      {
        value v;
        if( !s ) return v;
        ValueStringDataSet(&v, LPCWSTR(s), UINT(aux::wcslen(s)), UT_STRING_ERROR);
        return v;
      }

      bool is_undefined() const { return t == T_UNDEFINED; }
      bool is_bool() const { return t == T_BOOL; }
      bool is_int() const { return t == T_INT; }
      bool is_float() const { return t == T_FLOAT; }
      bool is_string() const { return t == T_STRING; }
      bool is_symbol() const { return t == T_STRING && u == UT_STRING_SYMBOL; }
      bool is_error_string() const { return t == T_STRING && u == UT_STRING_ERROR; }
      bool is_date() const { return t == T_DATE; }
      bool is_currency() const { return t == T_CURRENCY; }
      bool is_map() const { return t == T_MAP; }
      bool is_array() const { return t == T_ARRAY; }
      bool is_function() const { return t == T_FUNCTION; }
      bool is_bytes() const { return t == T_BYTES; }
      bool is_object() const { return t == T_OBJECT; }
      bool is_dom_element() const { return t == T_DOM_OBJECT; }
      // if it is a native functor reference
      bool is_native_function() const { return !!ValueIsNativeFunctor(this); }
      
      bool is_null() const { return t == T_NULL; }

      static value null() { value n; n.t = T_NULL; return n; }

      bool operator == (const value& rs) const 
      {
        if( this == &rs ) return true;
        int r = ValueCompare( this, &rs );
        if( r == HV_OK )
          return false;
        else if (r == HV_OK_TRUE)
          return true;
        else
          assert(false);
        return false;
      }
      bool operator != (const value& rs) const 
      {
        return !(operator==(rs));
      }


      int get(int defv) const 
      {
        int v;
        if(ValueIntData(this,&v) == HV_OK) return v; 
        return defv;
      }
      double get(double defv) const 
      {
        double v;
        if(ValueFloatData(this,&v) == HV_OK) return v; 
        return defv;
      }
      string get(const WCHAR* defv) const
      {
        aux::wchars wc;
        if(ValueStringData(this, (LPCWSTR*) &wc.start,&wc.length) == HV_OK)
          return aux::make_string(wc); 
        return string(defv);
      }
      aux::wchars get_chars() const
      {
        aux::wchars s;
        ValueStringData(this,(LPCWSTR*)&s.start,&s.length);
        return s;
      }
      aux::bytes get_bytes() const 
      {
        aux::bytes bs;
        ValueBinaryData(this,&bs.start,&bs.length);
        return bs;
      }

#ifdef WIN32
      FILETIME get_date() const
      { 
        INT64 v;
        if(ValueInt64Data(this,&v) == HV_OK) return *((FILETIME*)&v); 
        return FILETIME();
      }
#endif

      bool get(bool defv) const 
      {
        int v;
        if(ValueIntData(this,&v) == HV_OK) return v != 0; 
        return defv;
      }

      template<typename T> T get() const;

      static value from_string(const WCHAR* s, unsigned int len = 0, VALUE_STRING_CVT_TYPE ct = CVT_SIMPLE)
      {
        value t;
        if( s ) 
        {
          if(len == 0) len = (unsigned int)str_length(s);
          ValueFromString( &t, LPCWSTR(s), len, ct );
        }
        return t;
      }
      static value from_string(const std::basic_string<WCHAR>& s, VALUE_STRING_CVT_TYPE ct = CVT_SIMPLE)
      {
        return from_string(s.c_str(), (unsigned int)s.length(),ct);
      }
      static value from_string(aux::wchars s, VALUE_STRING_CVT_TYPE ct = CVT_SIMPLE)
      {
        return from_string(s.start, s.length,ct);
      }


      string to_string(int how = CVT_SIMPLE) const
      {
        if( how == CVT_SIMPLE && is_string() )
          return aux::make_string(get_chars()); // do not need to allocate
        value tv = *this;
        ValueToString(&tv,how);
        return aux::make_string(tv.get_chars());
      }

      void clear()
      {
        ValueClear(this);
      }

      // if it is an array or map returns number of elements there, otherwise - 0
      // if it is a function - returns number of arguments
      int length() const 
      {
        int n = 0;
        ValueElementsCount( this, &n);
        return n;
      }
      // if it is an array - returns nth element
      // if it is a map - returns nth value of the map
      // if it is a function - returns nth argument
      // otherwise it returns undefined value
      value get_item(int n) const
      {
        value r;
        ValueNthElementValue( this, n, &r);
        return r;
      }

      const value operator[](int n) const { return get_item(n); }
      value_idx_a operator[](int n);

      // if it is a map - returns value under the key in the map
      // if it is a function - returns value of argument with the name
      // otherwise it returns undefined value
      const value operator[](const value& key) const { return get_item(key); }
      value_key_a operator[](const value& key);

      typedef std::function<bool(const value& key, const value& val)> key_value_cb;

      struct enum_cb
      {
        // return true to continue enumeration
        virtual bool on(const value& key, const value& val) = 0;
        static BOOL SC_CALLBACK _callback( LPVOID param, const VALUE* pkey, const VALUE* pval )
        {
          enum_cb* cb = (enum_cb*)param;
          return cb->on( *(value*)pkey, *(value*)pval );
        }

        static BOOL SC_CALLBACK lambda_callback( LPVOID param, const VALUE* pkey, const VALUE* pval )
        {
          key_value_cb* cb = (key_value_cb*)param;
          return (*cb)(*(value*)pkey, *(value*)pval );
        }

      };

      // enum
      void enum_elements(enum_cb& cb) const
      {
        ValueEnumElements(const_cast<value*>(this), &enum_cb::_callback, &cb);
      }

      // calls cbf for each key/value pair found in T_OBJECT or T_MAP  
      void each_key_value(key_value_cb cbf) const
      {
        ValueEnumElements(const_cast<value*>(this), &enum_cb::lambda_callback, &cbf);
      }
      
      value key(int n) const
      {
        value r;
        ValueNthElementKey( this, n, &r);
        return r;
      }

      // if it is an array - sets nth element expanding the array if needed
      // if it is a map - sets nth value of the map;
      // if it is a function - sets nth argument of the function;
      // otherwise it converts this to array and adds v as first element.
      void set_item(int n, const value& v)
      {
        ValueNthElementValueSet( this, n, &v);
      }

      void append(const value& v) 
      {
        ValueNthElementValueSet( this, length(), &v);
      }
      // if it is a map - sets named value in the map;
      // if it is a function - sets named argument of the function;
      // otherwise it converts this to map and adds key/v to it.

      void set_item(const value& key, const value& v)
      {
        ValueSetValueToKey( this,&key,&v );
      }
      void set_item(const char* name, const value& v)
      {
        value key(name);
        ValueSetValueToKey( this,&key,&v );
      }

      /** get value by key value
          \return \b #value under that key if this value is a map/object containing that key, otherwise undefined value */
      value get_item(const value& key) const
      {
        value r;
        ValueGetValueOfKey( this, &key, &r);
        return r;
      }

      /** get value by name 
          \return \b #value under that key if this value is a map/object containing that key, otherwise undefined value */
      value get_item(const char* name) const
      {
        value key(name);
        value r;
        ValueGetValueOfKey( this, &key, &r);
        return r;
      }
      
      // T_OBJECT and T_DOM_OBJECT only, get value of object's data slot
      void* get_object_data() const
      {
        LPCBYTE pv = 0; unsigned int dummy;
        UINT r = ValueBinaryData(this,&pv,&dummy); r = r;
        assert(r == HV_OK);
        return (void*)pv;
      }

      //
      // Below this point are TISCRIPT/SCITER related methods
      //

//#if defined(HAS_TISCRIPT)

      bool is_object_native() const   {  return t == T_OBJECT && u == UT_OBJECT_NATIVE; }
      bool is_object_array() const    {  return t == T_OBJECT && u == UT_OBJECT_ARRAY; }
      bool is_object_function() const {  return t == T_OBJECT && u == UT_OBJECT_FUNCTION; }
      bool is_object_object() const   {  return t == T_OBJECT && u == UT_OBJECT_OBJECT; } // that is plain TS object
      bool is_object_class() const    {  return t == T_OBJECT && u == UT_OBJECT_CLASS; }  // that is TS class
      bool is_object_error() const    {  return t == T_OBJECT && u == UT_OBJECT_ERROR; }  // that is TS error

      
      // T_OBJECT only, set value of object's data slot
      void set_object_data(void* pv)
      {
        assert(u == UT_OBJECT_NATIVE);
        ValueBinaryDataSet(this,(LPCBYTE)pv,1,T_OBJECT,0);
      }
      
      // T_OBJECT only, class name of the object: e.g. Array, Function, etc. or custom class name.
      //std_wstring get_object_class_name() const
      //{
      //  assert(is_object());
      //  return get(L"");
      //}
      // T_OBJECT/UT_OBJECT_FUNCTION only, call TS function
      // 'self' here is what will be known as 'this' inside the function, can be undefined for invocations of global functions 
      value call( int argc, const value* argv, value self = value(), const WCHAR* url_or_script_name = 0) const
      {
        value rv;
        ValueInvoke(const_cast<value*>(this),&self,argc,argv,&rv,LPCWSTR(url_or_script_name));
        return rv;
      }

      value call() const {  return call(0,nullptr);  }
      value call( const value& p1 ) const {  return call(1,&p1); }
      value call( const value& p1, const value& p2 )  const { value args[2] = { p1,p2 };  return call(2,args); }
      value call( const value& p1, const value& p2, const value& p3 ) const { value args[3] = { p1,p2,p3 };  return call(3,args); }
      value call( const value& p1, const value& p2, const value& p3, const value& p4 )  const { value args[4] = { p1,p2,p3,p4 };  return call(4,args); }

      /** converts T_OBJECT/UT_OBJECT_*** values into plain map of key/value pairs */
      void isolate()
      {
        ValueIsolate(this);
      }

//#endif //defined(HAS_TISCRIPT)
          
      // "smart" or "soft" equality test
      static bool equal(const value& v1, const value& v2)
      {
        if( v1 == v2 ) return true; // strict comparison
        switch ( v1.t > v2.t? v1.t: v2.t )
        {
           case T_BOOL:
             {
               bool const r1 = v1.get(false);
               bool const r2 = v2.get(!r1);
               return r1 == r2;
             }
           case T_INT:
             {
               int const r1 = v1.get(0);
               int const r2 = v2.get(-r1);
               return r1 == r2;
             }
           case T_FLOAT:
             {
               double const r1 = v1.get(0.0);
               double const r2 = v2.get(-r1);
               return r1 == r2;
             }
        }
        return false;
      }
    };

    template<> inline int value::get<int>() const           { return get(0); }
    template<> inline unsigned value::get<unsigned>() const { return (unsigned)get(0); }
    template<> inline bool value::get<bool>() const         { return get(false); }
    template<> inline double value::get<double>() const     { return get(0.0); }
    template<> inline float value::get<float>() const       { return (float)get(0.0); }
    template<> inline string value::get<string>() const     { return to_string(); }
    //template<> inline const value& value::get<const value&>() const  { return *this; }
    template<> inline value value::get<value>() const       { return *this; }
      
    // value by key bidirectional proxy/accessor 
    class value_key_a
    {
      friend class value;
      value& col;
      value  key;
      value_key_a& operator=(const value_key_a& val); // no such thing
    protected:
      value_key_a( value& c, const value& k ): col(c),key(k) {}
    public:
      ~value_key_a() {}
      value_key_a& operator= (const value& val) { col.set_item(key,val); return *this; }
      operator const value() const              { return col.get_item(key); }
    };

    inline value_key_a 
        value::operator[](const value& key) { return value_key_a(*this, key); }

    // value by index bidirectional proxy/accessor 
    class value_idx_a
    {
      friend class value;
      value& col;
      int    idx;
      value_idx_a& operator= (const value_idx_a& val); // no such thing
    protected:
      value_idx_a( value& c, int i ): col(c), idx(i) {}
    public:
      ~value_idx_a() {}
      value_idx_a& operator= (const value& val) { col.set_item(idx,val); return *this; }
      operator const value() const              { return col.get_item(idx); }
    };

    inline value_idx_a 
        value::operator[](int idx) { return value_idx_a(*this, idx); }

  }


#ifdef CPP11
  namespace sciter {

    class native_function
    {
    public:
      native_function(const native_function_t& f): func(f) { assert(f); }
      virtual ~native_function() {}

      virtual bool invoke( unsigned int argc, const VALUE* argv, VALUE* retval ) 
      {
        if( func ) { 
          ValueInit(retval);
          value r = func(argc,static_cast<const value*>(argv));
          ValueCopy(retval,&r);
          return true;
        }
        else
          return false;
      }
      native_function_t func;
    private:
      native_function(const native_function& f);
      native_function& operator=(const native_function& f);
    public:
      static VOID invoke_impl( VOID* tag, UINT argc, const VALUE* argv, VALUE* retval) 
      {
        native_function* self = static_cast<native_function*>(tag);
        ValueInit(retval);
        value r = self->func(argc,static_cast<const value*>(argv));
        ValueCopy(retval,&r);
      }
      static VOID release_impl( VOID* tag ) 
      {
        native_function* self = static_cast<native_function*>(tag);
        delete self;
      }
    };

    inline value::value( const native_function_t& nfr ) {
      ValueInit(this); 
      native_function* pnf = new native_function(nfr);
      ValueNativeFunctorSet(this, native_function::invoke_impl, native_function::release_impl, pnf );
    }

    // vfunc(native function) is a wrapper that produces sciter::value from native function
    // see uminimal sample 
    template<typename R> 
    inline value vfunc( R(*func)() ) {
      return value([func](unsigned int argc, const value* argv) -> value { R r = func(); return value(r); }); 
    }
    template<typename R, typename T1> 
    inline value vfunc( R(*func)(T1 t1) ) {
      return value([func](unsigned int argc, const value* argv) -> value { 
          R r = func(argc >= 1? argv[0].get<T1>(): T1() ); 
          return value(r); 
      }); 
    }
    template<typename R, typename T1, typename T2> 
    inline value vfunc( R(*func)(T1 t1,T2 t2) ) {
      return value([func](unsigned int argc, const value* argv) -> value { 
          R r = func(argc >= 1? argv[0].get<T1>(): T1(),
                    argc >= 2? argv[1].get<T2>(): T2() ); 
          return value(r); 
      }); 
    }
    template<typename R, typename T1, typename T2, typename T3> 
    inline value vfunc( R(*func)(T1 t1,T2 t2,T3 t3) ) {
      return value([func](unsigned int argc, const value* argv) -> value { 
          R r = func(argc >= 1? argv[0].get<T1>(): T1(),
                    argc >= 2? argv[1].get<T2>(): T2(),
                    argc >= 3? argv[2].get<T3>(): T3()); 
          return value(r); 
      }); 
    }
    template<typename R, typename T1, typename T2, typename T3, typename T4> 
    inline value vfunc( R(*func)(T1 t1,T2 t2,T3 t3,T4 t4) ) {
      return value([func](unsigned int argc, const value* argv) -> value { 
          R r = func(argc >= 1? argv[0].get<T1>(): T1(),
                    argc >= 2? argv[1].get<T2>(): T2(),
                    argc >= 3? argv[2].get<T3>(): T3(),
                    argc >= 4? argv[3].get<T4>(): T4()); 
          return value(r); 
      }); 
    }

    // versions of the above but for generic std::function
    template<typename R>
      inline value vfunc( std::function<R()> func ) 
      {
         return value([func](unsigned int argc, const value* argv) -> value { 
            R r = func(); return value(r); 
         }); 
      }

    template<typename R,typename P0>
      inline value vfunc( std::function<R(P0)> func )
      {
        return value([func](unsigned int argc, const value* argv) -> value { 
          R r = func(argc >= 1? argv[0].get<P0>(): P0() ); 
          return value(r); }); 
      }
    template<typename R,typename P0,typename P1>
      inline value vfunc( std::function<R(P0,P1)> func )
      {
        return value([func](unsigned int argc, const value* argv) -> value { 
          R r = func(argc >= 1? argv[0].get<P0>(): P0(),
                     argc >= 2? argv[1].get<P1>(): P1() ); 
          return value(r); }); 
      }

    template<typename R, typename P0, typename P1, typename P2> 
      inline value vfunc( std::function<R(P0,P1,P2)> func ) 
      {
        return value([func](unsigned int argc, const value* argv) -> value { 
          R r = func(argc >= 1? argv[0].get<P0>(): P0(),
                    argc >= 2? argv[1].get<P1>(): P1(),
                    argc >= 3? argv[2].get<P2>(): P2()); 
          return value(r); }); 
      }

    template<typename R, typename P0, typename P1, typename P2, typename P3> 
      inline value vfunc( std::function<R(P0,P1,P2,P3)> func ) {
        return value([func](unsigned int argc, const value* argv) -> value { 
            R r = func(argc >= 1? argv[0].get<P0>(): P0(),
                      argc >= 2? argv[1].get<P1>(): P1(),
                      argc >= 3? argv[2].get<P2>(): P2(),
                      argc >= 4? argv[3].get<P3>(): P3()); 
            return value(r); }); 
      }

      
  }
#endif

  #pragma warning( pop )

#endif