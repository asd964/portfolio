using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.IO;
using System.IO.Compression;
using System.Text.RegularExpressions;
using Newtonsoft.Json.Linq;
using Newtonsoft.Json;
namespace Download.Controllers
{
    public class HomeController : Controller
    {



        public string Index()
        {
            // 검증 ok                 

            return "thie is File Download function";
        }

        public ActionResult download(int SerialN1 , int SerialN2 , string product , string marginal = "empty")
        {   
            try
            {
               
                Guid guid = Guid.NewGuid();
                string randNum = guid.ToString();
                if (!Directory.Exists(Server.MapPath("~/FileDownload/"+product)))
                {
                    return Json(new {result = "fail", returnCode = 1, msg = "Product serial number "+product+" is not exist."}, JsonRequestBehavior.AllowGet);
                }
                string[] files = Directory.GetFiles(Server.MapPath("~/FileDownload/"+product));
                List<string> downloads = new List<string>();

                if (!System.IO.File.Exists(Server.MapPath
                                  ("~/FileDownload/"+product+"/calibration_data_cam" + SerialN1 + ".cal"))&&
                    !System.IO.File.Exists(Server.MapPath
                                  ("~/FileDownload/" + product + "/calibration_data_cam" + SerialN2 + ".cal")))
                    return Json(new { result = "fail", returnCode = 2, msg = "Camera serial numbers " + SerialN1 + " , " + SerialN2 + " are not exist." }, JsonRequestBehavior.AllowGet);
                else if(!System.IO.File.Exists(Server.MapPath
                                  ("~/FileDownload/" + product + "/calibration_data_cam" + SerialN1 + ".cal")))
                    return Json(new { result = "fail", returnCode = 2, msg = "Camera serial number " + SerialN1 + " is not exist." }, JsonRequestBehavior.AllowGet);
                else if(!System.IO.File.Exists(Server.MapPath
                                  ("~/FileDownload/" + product + "/calibration_data_cam" + SerialN2 + ".cal")))
                    return Json(new { result = "fail", returnCode = 2, msg = "Camera serial number " + SerialN2 + " is not exist." }, JsonRequestBehavior.AllowGet);
                else
                {
                    foreach(string file in files)
                    {
                            downloads.Add(Path.GetFileName(file));
                    }           
                }

                if (System.IO.File.Exists(Server.MapPath
                                  ("~/zipfiles/" + randNum + ".zip")))
                {
                    System.IO.File.Delete(Server.MapPath
                                  ("~/zipfiles/" + randNum + ".zip"));
                }

                ZipArchive zip = ZipFile.Open(Server.MapPath
                        ("~/zipfiles/" + randNum + ".zip"), ZipArchiveMode.Create);
                foreach (string file in downloads)
                {
                    zip.CreateEntryFromFile(Server.MapPath
                       ("~//" + product + "//" + file), file);
                }
                //zip.ExtractToDirectory(Server.MapPath("~/image/"));
                zip.Dispose();

                return File(Server.MapPath("~/zipfiles/" + randNum + ".zip"),
                          "application/zip", "calibration_data_cam" + randNum + ".zip");
                }
                catch(Exception e)
                {
                    string msg = e.Message;
                    return Json(new { result = "fail", returnCode = 0, msg = msg }, JsonRequestBehavior.AllowGet);
                }
            }


        
        public ActionResult DownloadFile(int SerialN1 , string product , string marginal = "empty")
        {      
            try
            {
                string fileName = "calibration_data_cam" + SerialN1 + ".cal";
                
                //if (!Directory.Exists(Server.MapPath("~/FileDownload/" + product)) || !System.IO.File.Exists(Server.MapPath
                //                  ("~/FileDownload/" + product + "/calibration_data_cam" + SerialN1 + ".cal")))
                //{
                //    List<String> result = searchSHPFiles(Server.MapPath("~/FileDownload/"));
                //    foreach (string file_path in result)
                //    {
                //        if (file_path.Contains(fileName))
                //            return File(file_path, MimeMapping.GetMimeMapping(file_path), fileName);
                    
                //    }
                //return Json(new { result = "fail", returnCode = 2, msg = "Camera serial number " + SerialN1 + " is not exist." }, JsonRequestBehavior.AllowGet);
                ////전체 디렉토리를 검색했을 때 파일이 없으면 Json Return;
                //}
                                               
                
                
                    if (!Directory.Exists(Server.MapPath("~/FileDownload/" + product)))
                    {
                        return Json(new { result = "fail", returnCode = 1, msg = "Product serial number " + product + " is not exist." }, JsonRequestBehavior.AllowGet);
                    }
                    string[] files = Directory.GetFiles(Server.MapPath("~/FileDownload/" + product));

                    if (!System.IO.File.Exists(Server.MapPath
                                      ("~/FileDownload/" + product + "/calibration_data_cam" + SerialN1 + ".cal")))
                        return Json(new { result = "fail", returnCode = 2, msg = "Camera serial number " + SerialN1 + " is not exist." }, JsonRequestBehavior.AllowGet);
                    
                var filepath = System.IO.Path.Combine(Server.MapPath("~/FileDownload/" + product + "/"), fileName);
                    return File(filepath, MimeMapping.GetMimeMapping(filepath), fileName);
            }
            catch (Exception e)
            {
                string msg = e.Message;
                return Json(new { result = "fail", returnCode = 0, msg = msg }, JsonRequestBehavior.AllowGet);
            }
        }



        public void DirFileSearch(string path, string file)
        {
        }





        List<String> searchSHPFiles(string sDir)
        {
            List<String> result = new List<String>();
            String filter = "*.cal";

            try
            {
                foreach (string d in Directory.GetDirectories(sDir))
                {
                    foreach (string f in Directory.GetFiles(d, filter))
                    {
                        result.Add(f);
                    }

                    searchSHPFiles(d);
                }
            }
            catch (System.Exception)
            {
                return null;
            }

            return result;
        }


    }
}