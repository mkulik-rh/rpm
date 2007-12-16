#ifndef _RPMFILEUTIL_H
#define _RPMFILEUTIL_H

/** \ingroup rpmfileutil rpmio
 * \file rpmio/rpmfileutil.h
 * File and path manipulation helper functions.
 */

#include <rpm/rpmio.h>
#include <rpm/rpmpgp.h>

/** \ingroup rpmfileutil
 */
typedef enum rpmCompressedMagic_e {
    COMPRESSED_NOT		= 0,	/*!< not compressed */
    COMPRESSED_OTHER		= 1,	/*!< gzip can handle */
    COMPRESSED_BZIP2		= 2,	/*!< bzip2 can handle */
    COMPRESSED_ZIP		= 3,	/*!< unzip can handle */
    COMPRESSED_LZMA		= 4	/*!< lzma can handle */
} rpmCompressedMagic;

/** \ingroup rpmfileutil
 * Calculate a file digest and size.
 * @param algo		digest algorithm (ignored for now, md5 used)
 * @param fn		file name
 * @param asAscii	return checksum as ascii string?
 * @retval digest	address of calculated checksum
 * @retval *fsizep	file size pointer (or NULL)
 * @return		0 on success, 1 on error
 */
int rpmDoDigest(pgpHashAlgo algo, const char * fn,int asAscii,
		  unsigned char * digest, size_t * fsizep);


/** \ingroup rpmfileutil
 * Return file handle for a temporaray file.
 * A unique temporaray file path will be generated using
 *	rpmGenPath(prefix, "%{_tmppath}/", "rpm-tmp.XXXXX")
 * where "XXXXXX" is filled in using rand(3). The file is opened, and
 * the link count and (dev,ino) location are verified after opening.
 * The file name and the open file handle are returned.
 *
 * @param prefix	leading part of temp file path
 * @retval fnptr	temp file name (or NULL)
 * @retval fdptr	temp file handle
 * @return		0 on success
 */
int rpmMkTempFile(const char * prefix, char ** fnptr, FD_t * fdptr);

/** \ingroup rpmfileutil
 * Insure that directories in path exist, creating as needed.
 * @param path		directory path
 * @param mode		directory mode (if created)
 * @param uid		directory uid (if created), or -1 to skip
 * @param gid		directory uid (if created), or -1 to skip
 * @return		0 on success, errno (or -1) on error
 */
int rpmioMkpath(const char * path, mode_t mode, uid_t uid, gid_t gid);

/** \ingroup rpmfileutil
 * Return type of compression used in file.
 * @param file		name of file
 * @retval compressed	address of compression type
 * @return		0 on success, 1 on I/O error
 */
int	isCompressed	(const char * file,
				rpmCompressedMagic * compressed);

/** \ingroup rpmfileutil
 * Canonicalize file path.
 * @param path		path to canonicalize (in-place)
 * @return		pointer to path
 */
char * rpmCleanPath	(char * path);

/** \ingroup rpmfileutil
 * Merge 3 args into path, any or all of which may be a url.
 * The leading part of the first URL encountered is used
 * for the result, other URL prefixes are discarded, permitting
 * a primitive form of URL inheiritance.
 * @param urlroot	root URL (often path to chroot, or NULL)
 * @param urlmdir	directory URL (often a directory, or NULL)
 * @param urlfile	file URL (often a file, or NULL)
 * @return		expanded, merged, canonicalized path (malloc'ed)
 */
char * rpmGenPath	(const char * urlroot,
			const char * urlmdir,
			const char * urlfile);

/** \ingroup rpmfileutil
 * Return (malloc'ed) expanded, canonicalized, file path.
 * @param path		macro(s) to expand (NULL terminates list)
 * @return		canonicalized path (malloc'ed)
 */
char * rpmGetPath (const char * path, ...);

/** \ingroup rpmfileutil
 * Return URL path(s) from a (URL prefixed) pattern glob.
 * @param patterns	glob pattern
 * @retval *argcPtr	no. of paths
 * @retval *argvPtr	array of paths (malloc'd contiguous blob)
 * @return		0 on success
 */
int rpmGlob(const char * patterns, int * argcPtr,
		const char *** argvPtr);

#endif /* _RPMFILEUTIL_H */
