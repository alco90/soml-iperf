Name: oml2-iperf
Version: 2.0.5+oml1
Release: 3%{?dist}
Summary: Iperf with OML support
License: BSD
Group: Applications/Internet
URL: http://oml.mytestbed.net/projects/iperf
Source: http://oml.mytestbed.net/attachments/download/639/%{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
This project is meant to implement OML support following Iperf development. 
It is based on iperf 2.0.5.

%prep
%setup -q

%build
%configure --program-prefix=oml2-
%{__make} %{?_smp_mflags}

%install
%{__rm} -rf %{buildroot}
%make_install

%clean
%{__rm} -rf %{buildrootndir}}

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog COPYING README doc/*.gif doc/*.html
%{_bindir}/%{name}
%{_mandir}/man*/*
%{_datadir}/%{name}
