using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Configuration.Install;

namespace RaumserverServiceNET
{
    [RunInstaller(true)]
    public class ServiceWinInstaller : Installer
    {
        private ServiceInstaller m_ThisService;
        private ServiceProcessInstaller m_ThisServiceProcess;

        public ServiceWinInstaller()
        {
            m_ThisService = new ServiceInstaller();
            m_ThisServiceProcess = new ServiceProcessInstaller();

            m_ThisServiceProcess.Account = ServiceAccount.LocalService;
            m_ThisService.ServiceName = "Raumserver Service";
            m_ThisService.StartType = ServiceStartMode.Automatic;

            Installers.Add(m_ThisService);
            Installers.Add(m_ThisServiceProcess);
        }
    }


    public enum ServiceState
    {
        SERVICE_STOPPED = 0x00000001,
        SERVICE_START_PENDING = 0x00000002,
        SERVICE_STOP_PENDING = 0x00000003,
        SERVICE_RUNNING = 0x00000004,
        SERVICE_CONTINUE_PENDING = 0x00000005,
        SERVICE_PAUSE_PENDING = 0x00000006,
        SERVICE_PAUSED = 0x00000007,
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ServiceStatus
    {
        public long dwServiceType;
        public ServiceState dwCurrentState;
        public long dwControlsAccepted;
        public long dwWin32ExitCode;
        public long dwServiceSpecificExitCode;
        public long dwCheckPoint;
        public long dwWaitHint;
    };

   
    public partial class RaumserverService : ServiceBase
    {

        [DllImport("advapi32.dll")] private static extern bool SetServiceStatus(IntPtr hServiceStatus, ref ServiceStatus lpServiceStatus);

        [DllImport("raumserver.dll")] public static extern IntPtr createRaumserverObject();
        [DllImport("raumserver.dll")] public static extern void deleteRaumserverObject(IntPtr value);
        [DllImport("raumserver.dll")] public static extern void initRaumserverObject(IntPtr value);


        IntPtr raumserver;

        public RaumserverService()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            // Update the service state to Start Pending.
            ServiceStatus serviceStatus = new ServiceStatus();
            serviceStatus.dwCurrentState = ServiceState.SERVICE_START_PENDING;
            serviceStatus.dwWaitHint = 100000;
            SetServiceStatus(this.ServiceHandle, ref serviceStatus);


            raumserver = createRaumserverObject();
            initRaumserverObject(raumserver);
            // TODO: init Raumkernel

            // Update the service state to Running.
            serviceStatus.dwCurrentState = ServiceState.SERVICE_RUNNING;
            SetServiceStatus(this.ServiceHandle, ref serviceStatus);
        }

        protected override void OnStop()
        {
            deleteRaumserverObject(raumserver);
            // delete rumkernel
        }
    }
}
